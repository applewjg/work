#! /usr/bin/env python
# -*- coding: utf-8 -*-

def foo(): 
	a = [0] 
	def bar(x): 
		a[0] = a[0] + x
		return a[0] 
	return bar

fun1 = foo()
fun2 = foo()

for i in xrange(10):
	print fun1(i), fun2(-i)

