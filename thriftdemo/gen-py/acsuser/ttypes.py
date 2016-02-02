#
# Autogenerated by Thrift Compiler (0.9.2)
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#
#  options string: py
#

from thrift.Thrift import TType, TMessageType, TException, TApplicationException

from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol, TProtocol
try:
  from thrift.protocol import fastbinary
except:
  fastbinary = None



class User:
  """
  Attributes:
   - uid
   - uname
   - usex
   - uage
  """

  thrift_spec = (
    None, # 0
    (1, TType.STRING, 'uid', None, None, ), # 1
    (2, TType.STRING, 'uname', None, None, ), # 2
    (3, TType.BOOL, 'usex', None, None, ), # 3
    (4, TType.I16, 'uage', None, None, ), # 4
  )

  def __init__(self, uid=None, uname=None, usex=None, uage=None,):
    self.uid = uid
    self.uname = uname
    self.usex = usex
    self.uage = uage

  def read(self, iprot):
    if iprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None and fastbinary is not None:
      fastbinary.decode_binary(self, iprot.trans, (self.__class__, self.thrift_spec))
      return
    iprot.readStructBegin()
    while True:
      (fname, ftype, fid) = iprot.readFieldBegin()
      if ftype == TType.STOP:
        break
      if fid == 1:
        if ftype == TType.STRING:
          self.uid = iprot.readString();
        else:
          iprot.skip(ftype)
      elif fid == 2:
        if ftype == TType.STRING:
          self.uname = iprot.readString();
        else:
          iprot.skip(ftype)
      elif fid == 3:
        if ftype == TType.BOOL:
          self.usex = iprot.readBool();
        else:
          iprot.skip(ftype)
      elif fid == 4:
        if ftype == TType.I16:
          self.uage = iprot.readI16();
        else:
          iprot.skip(ftype)
      else:
        iprot.skip(ftype)
      iprot.readFieldEnd()
    iprot.readStructEnd()

  def write(self, oprot):
    if oprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and self.thrift_spec is not None and fastbinary is not None:
      oprot.trans.write(fastbinary.encode_binary(self, (self.__class__, self.thrift_spec)))
      return
    oprot.writeStructBegin('User')
    if self.uid is not None:
      oprot.writeFieldBegin('uid', TType.STRING, 1)
      oprot.writeString(self.uid)
      oprot.writeFieldEnd()
    if self.uname is not None:
      oprot.writeFieldBegin('uname', TType.STRING, 2)
      oprot.writeString(self.uname)
      oprot.writeFieldEnd()
    if self.usex is not None:
      oprot.writeFieldBegin('usex', TType.BOOL, 3)
      oprot.writeBool(self.usex)
      oprot.writeFieldEnd()
    if self.uage is not None:
      oprot.writeFieldBegin('uage', TType.I16, 4)
      oprot.writeI16(self.uage)
      oprot.writeFieldEnd()
    oprot.writeFieldStop()
    oprot.writeStructEnd()

  def validate(self):
    return


  def __hash__(self):
    value = 17
    value = (value * 31) ^ hash(self.uid)
    value = (value * 31) ^ hash(self.uname)
    value = (value * 31) ^ hash(self.usex)
    value = (value * 31) ^ hash(self.uage)
    return value

  def __repr__(self):
    L = ['%s=%r' % (key, value)
      for key, value in self.__dict__.iteritems()]
    return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

  def __eq__(self, other):
    return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

  def __ne__(self, other):
    return not (self == other)
