"""
Calculating coordinate from distances to references
"""

import os
import sys
from subprocess import Popen, PIPE
bin =  'ei-coord_server'

class CoordinateSolverStartupError(Exception):
	pass

class SolverError(Exception):
	pass

class CoordinateSolver(object):
	"""The fingerprint calculator wrapper class"""
	def __init__(self, reference):
		"""start the process"""
		self.reference=reference
		self.start()
	
	def start(self):
		try:
			self.child = Popen([bin,self.reference],stdin=PIPE,stdout=PIPE)
		except Exception as e :
			print "error: "+str(e)
			try: self.child.kill()
			except: pass
			sys.stderr.write("Error: Cannot start %s\n"%bin)
			raise CoordinateSolverStartupError
	
	def close(self):
		self.child.kill()
	
	def solve(self, line):
		if self.child.poll():
			self.start()

		self.child.stdin.write(line+"\n")
		fp = self.child.stdout.readline()
		if self.child.poll():
			raise SolverError("return code: %d" % self.child.returncode())
		return fp
