#!/usr/bin/env python

import os
import os.path
import tempfile
import subprocess
import time
import signal
import re
import sys
import shutil

# 若不用wsl的远程连接才能正常运行，这是因为该文件存在windows的目录下却运行在wsl的远程连接中，存在部分权限问题，因而以后最好直接全部在其中一个系统中运行
# 但现在仍未知道为什么这个lab放在wsl的目录下用远程连接会错误运行，但鉴于在windows能跑就先搁置了
file_locations = os.getcwd()
# file_locations = os.getcwd()
# logisim_location = os.path.join("D:/cs61c/lab05/testing/logisim-evolution.jar")
logisim_location = os.path.join(os.getcwd(), "logisim-evolution.jar")
print(f"DEBUG: Current working directory in test.py: {os.getcwd()}") # 打印当前工作目录
print(f"DEBUG: Attempting to use Logisim from: {logisim_location}") # 打印 Logisim 路径
print(f"DEBUG: Does Logisim JAR exist at this path? {os.path.exists(logisim_location)}") # 检查文件是否存在

class TestCase():
  """
      Runs specified circuit file and compares output against the provided reference trace file.
  """

  def __init__(self, circfile, tracefile, register_doc):
    self.circfile  = circfile
    self.tracefile = tracefile
    self.register_doc = register_doc

  def __call__(self, filename):
    output = tempfile.TemporaryFile(mode='r+')
    try:
      stdinf = open('/dev/null')
    except Exception as e:
      try:
        stdinf = open('nul')
      except Exception as e:
         print("Could not open nul or /dev/null. Program will most likely error now.")
    proc = subprocess.Popen(["java","-jar",logisim_location,"-tty","table",self.circfile], stdin=stdinf, stdout=subprocess.PIPE)
    try:
      reference = open(self.tracefile)
      passed = compare_unbounded(proc.stdout,reference, filename)
    finally:
      try: 
        os.kill(proc.pid,signal.SIGTERM)
      except Exception as e: 
        pass
    if passed:
      return (True, "Matched expected output")
    else:
      return (False, "Did not match expected output")

def compare_unbounded(student_out, reference_out, filename):
  passed = True
  student_output_array = []
  while True:
    line1 = student_out.readline().rstrip().decode("utf-8", "namereplace")
    line2 = reference_out.readline().rstrip()
    if line2 == '':
      break
    student_output_array.append(line1)
    m = re.match(line2, line1)
    if m == None or m.start() != 0 or m.end() != len(line2):
      passed = False
  with open(filename, "w") as student_output:
    for line in student_output_array:
      student_output.write(line + '\n')
  return passed


def run_tests(tests):
  print("Testing files...")
  tests_passed = 0
  tests_failed = 0

  for description,filename,test in tests:
    test_passed, reason = test(filename)
    if test_passed:
      print("\tPASSED test: %s" % description)
      tests_passed += 1
    else:
      print("\tFAILED test: %s (%s)" % (description, reason))
      tests_failed += 1
  
  print("Passed %d/%d tests" % (tests_passed, (tests_passed + tests_failed)))

tests = [
  ("Exercise 1 (Sub-Circuits) test", "student_output/ex1_test.out",TestCase(os.path.join(file_locations,'circ_files/ex1_test.circ'), os.path.join
(file_locations,'reference_output/ex1_test.out'), "")),
  ("Exercise 2 (Add Machine) test", "student_output/ex2_test.out",TestCase(os.path.join(file_locations,'circ_files/ex2_test.circ'), os.path.join
(file_locations,'reference_output/ex2_test.out'), "")),
  ("Exercise 3 (FSM) test", "student_output/ex3_test.out",TestCase(os.path.join(file_locations,'circ_files/ex3_test.circ'), os.path.join
(file_locations,'reference_output/ex3_test.out'), "")),
  ("Exercise 4 (Split) test", "student_output/ex4_test.out",TestCase(os.path.join(file_locations,'circ_files/ex4_test.circ'), os.path.join
(file_locations,'reference_output/ex4_test.out'), "")),
  ("Exercise 5 (Rotate Right) test", "student_output/ex5_test.out",TestCase(os.path.join(file_locations,'circ_files/ex5_test.circ'), os.path.join
(file_locations,'reference_output/ex5_test.out'), "")),
]

if __name__ == '__main__':
  run_tests(tests)
