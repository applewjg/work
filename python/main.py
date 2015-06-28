#!/usr/bin/python
# -*- coding: utf8 -*-

def dec(func):
  # when decorator is called without param,
  # the decorated func shoud be the param
  print "before myfunc() called."
  func()
  print "after myfunc() called."
  return func

def dec1(func):
  # when decorator is called without param,
  # the decorated func shoud be the param
  def _dec1():
    print "before myfunc() called."
    func()
    print "after myfunc() called."
  return _dec1

def dec2(func):
  def _deco(a, b):
    print("before myfunc() called.")
    ret = func(a, b)
    print("  after myfunc() called. result: %s" % ret)
    return ret
  return _deco

@dec2
def myfunc(a, b):
  print(" myfunc(%s,%s) called." % (a, b))
  return a + b

myfunc(1, 2)
myfunc(3, 4)


def decoWithArgs(arg):
  """
  由于有参数的decorator函数在调用时只会使用应用时的参数而不接收被装饰的函数做为参数，
  所以必须返回一个decorator函数， 由它对被装饰的函数进行封装处理
  """
  def newDeco(func):    #定义一个新的decorator函数
    def replaceFunc():    #在decorator函数里面再定义一个内嵌函数，由它封装具体的操作
      print "Enter decorator %s" %arg    #进行额外操作
      return func()    #对被装饰函数进行调用
    return replaceFunc
  return newDeco    #返回一个新的decorator函数

@decoWithArgs("demo")
def MyFunc():    #应用@decoWithArgs修饰的方法
  print "Enter MyFunc"

MyFunc()    #调用被装饰的函数

class locker:
    def __init__(self):
        print("locker.__init__() should be not called.")

    @staticmethod
    def acquire():
        print("locker.acquire() called.（这是静态方法）")

    @staticmethod
    def release():
        print("  locker.release() called.（不需要对象实例）")

def deco(cls):
    '''cls 必须实现acquire和release静态方法'''
    def _deco(func):
        def __deco():
            print("before %s called [%s]." % (func.__name__, cls))
            cls.acquire()
            try:
                return func()
            finally:
                cls.release()
        return __deco
    return _deco

@deco(locker)
def myfunc():
    print(" myfunc() called.")

myfunc()
myfunc()


'''''示例6: 对参数数量不确定的函数进行装饰，
参数用(*args, **kwargs)，自动适应变参和命名参数'''

def dec3(func):
    def _deco(*args, **kwargs):
        print("before %s called." % func.__name__)
        ret = func(*args, **kwargs)
        print("  after %s called. result: %s" % (func.__name__, ret))
        return ret
    return _deco

@dec3
def myfunc(a, b):
    print(" myfunc(%s,%s) called." % (a, b))
    return a+b

@dec3
def myfunc2(a, b, c):
    print(" myfunc2(%s,%s,%s) called." % (a, b, c))
    return a+b+c

myfunc(1, 2)
myfunc(3, 4)
myfunc2(1, 2, 3)
myfunc2(3, 4, 5)



class mylocker:
    def __init__(self):
        print("mylocker.__init__() called.")

    @staticmethod
    def acquire():
        print("mylocker.acquire() called.")

    @staticmethod
    def unlock():
        print("  mylocker.unlock() called.")

class lockerex(mylocker):
    @staticmethod
    def acquire():
        print("lockerex.acquire() called.")

    @staticmethod
    def unlock():
        print("  lockerex.unlock() called.")

def lockhelper(cls):
    '''''cls 必须实现acquire和release静态方法'''
    def _deco(func):
        def __deco(*args, **kwargs):
            print("before %s called." % func.__name__)
            cls.acquire()
            try:
                return func(*args, **kwargs)
            finally:
                cls.unlock()
        return __deco
    return _deco

class example:
    @lockhelper(mylocker)
    def myfunc(self):
        print(" myfunc() called.")

    @lockhelper(mylocker)
    @lockhelper(lockerex)
    def myfunc2(self, a, b):
        print(" myfunc2() called.")
        return a + b

if __name__=="__main__":
    print '##############hello world'
    a = example()
    a.myfunc()
    print(a.myfunc())
    print(a.myfunc2(1, 2))
    print(a.myfunc2(3, 4))
