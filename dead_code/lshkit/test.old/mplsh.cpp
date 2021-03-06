/* 
    Copyright (C) 2008 Wei Dong <wdong@princeton.edu>. All Rights Reserved.
  
    This file is part of LSHKIT.
  
    LSHKIT is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LSHKIT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LSHKIT.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <boost/program_options.hpp>
#include <boost/progress.hpp>
#include <boost/dynamic_bitset.hpp>
#include <lshkit/mplsh.h>
#include <lshkit/matrix.h>
#include <lshkit/eval.h>
#include <sys/time.h> 

class Timer
{
	struct	timeval	start; 
public:
    Timer () {}
    void tick ()
    {
	    gettimeofday(&start, 0); 
    }
    void tuck (const char *msg) const
    {
        struct timeval end;
	    float	diff; 
	    gettimeofday(&end, 0); 

	    diff = (end.tv_sec - start.tv_sec) 
	   			+ (end.tv_usec - start.tv_usec) * 0.000001; 
        std::cout << msg << ':' <<  diff << std::endl;
    }
};


using namespace std;
using namespace lshkit;
namespace po = boost::program_options; 

class MatrixAccessor
{
    const Matrix<float> &matrix_;
    boost::dynamic_bitset<> flags_;
public:
    typedef unsigned Key;
    MatrixAccessor(const Matrix<float> &matrix)
        : matrix_(matrix), flags_(matrix.getSize()) {}

    MatrixAccessor(const MatrixAccessor &accessor)
        : matrix_(accessor.matrix_), flags_(matrix_.getSize()) {}

    void reset ()
    {
        flags_.reset();
    }

    bool mark (unsigned key)
    {
        if (flags_[key]) return false;
        flags_.set(key);
        return true;
    }

    const float *operator () (unsigned key)
    {
        return matrix_[key];
    }
};

int main (int argc, char *argv[])
{
    string data_file;
    string benchmark;

    float W, R = 1.0;
    unsigned M, L, H;
    unsigned Q, K, T;
    bool do_recall = false;

    Timer timer;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message.")
		(",W", po::value<float>(&W)->default_value(1.0), "")
		(",M", po::value<unsigned>(&M)->default_value(1), "")
		(",L", po::value<unsigned>(&L)->default_value(1), "")
		(",H", po::value<unsigned>(&H)->default_value(1017881), "")
		(",Q", po::value<unsigned>(&Q)->default_value(1), "")
		(",K", po::value<unsigned>(&K)->default_value(1), "")
		(",T", po::value<unsigned>(&T)->default_value(1), "")
		("recall,R", po::value<float>(&R), "")
		("data,D", po::value<string>(&data_file), "")
		("benchmark,B", po::value<string>(&benchmark), "")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);	

	if (vm.count("help") || (vm.count("data") < 1) || (vm.count("benchmark") < 1))
	{
		cout << desc;
		return 0;
	}

    if (vm.count("recall") >= 1)
    {
        do_recall = true;
    }

    cout << "Loading data...";
    Matrix<float> data(data_file);
    cout << "done." << endl;

    Benchmark<> bench(K, Q);
    cout << "Loading benchmark...";
    bench.load(benchmark);
    cout << "done." << endl;

    for (unsigned i = 0; i < Q; ++i)
    {
        for (unsigned j = 0; j < K; ++j)
        {
            assert(bench.getAnswer(i)[j].key < data.getSize());
				}
    }

    cout << "Initializing index..." << endl;

    typedef MultiProbeLshIndex<MatrixAccessor> Index;
        
    Index::Parameter param;

    param.W = W;
    param.H = H;
    param.M = M;
    param.dim = data.getDim();
    DefaultRng rng;

    MatrixAccessor accessor(data);

    Index index(param, rng, accessor, L);

    cout << "done." << endl;

    cout << "Populating index..." << endl;

    timer.tick();

    {
        boost::progress_display progress(data.getSize());
        for (unsigned i = 0; i < data.getSize(); ++i)
        {
            index.insert(i);
            ++progress;
        }
    }
    timer.tuck("CREATE");

    cout << "Running queries..." << endl;

    Stat recall;
    Stat cost;
    Topk<unsigned> topk;

    timer.tick();
    if (do_recall)
    {
        boost::progress_display progress(Q);
        for (unsigned i = 0; i < Q; ++i)
        {
            unsigned cnt;
            topk.reset(K);
            index.query(data[bench.getQuery(i)], topk, R, &cnt);
            recall << bench.getAnswer(i).recall(topk);
            cost << double(cnt)/double(data.getSize());
            ++progress;
        }
    }
    else
    {
        boost::progress_display progress(Q);
        for (unsigned i = 0; i < Q; ++i)
        {
            unsigned cnt;
            topk.reset(K);
            index.query(data[bench.getQuery(i)], topk, T, &cnt);
            recall << bench.getAnswer(i).recall(topk);
            cost << double(cnt)/double(data.getSize());
            ++progress;
        }
    }
    timer.tuck("QUERY");

    cout << "[RECALL] " << recall.getAvg() << " � " << recall.getStd() << endl;
    cout << "[COST] " << cost.getAvg() << " � " << cost.getStd() << endl;

    return 0;
}

