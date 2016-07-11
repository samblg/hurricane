/**
 * licensed to the apache software foundation (asf) under one
 * or more contributor license agreements.  see the notice file
 * distributed with this work for additional information
 * regarding copyright ownership.  the asf licenses this file
 * to you under the apache license, version 2.0 (the
 * "license"); you may not use this file except in compliance
 * with the license.  you may obtain a copy of the license at
 *
 * http://www.apache.org/licenses/license-2.0
 *
 * unless required by applicable law or agreed to in writing, software
 * distributed under the license is distributed on an "as is" basis,
 * without warranties or conditions of any kind, either express or implied.
 * see the license for the specific language governing permissions and
 * limitations under the license.
 */

from ctypes import *

pyHurricaneHandle = CDLL('PyHurricane.dll')

class CValueInner(Union):
    _fields_ = [('booleanValue', c_byte),
        ('characterValue', c_char),
        ('int8Value', c_byte),
        ('int16Value', c_short),
        ('int32Value', c_int),
        ('int64Value', c_longlong),
        ('floatValue', c_float),
        ('doubleValue', c_double),
        ('stringValue', c_char_p)]

class CValue(Structure):
    Boolean = 0
    Character = 1
    Int8 = 2
    Int16 = 3
    Int32 = 4
    Int64 = 5
    Float = 6
    Double = 7
    String = 8
    _fields_ = [('type', c_byte),
        ('value', CValueInner),
        ('length', c_int)]

ValuePointer = POINTER(CValue)

class CValues(Structure):
    _fields_ = [('length', c_int),
            ('values', ValuePointer)]

class Task(object):
    CloneHandler = CFUNCTYPE(c_int)
    Emitter = CFUNCTYPE(c_void_p, c_void_p)

    @staticmethod
    def fromCValues(cValues):
        values = []
        length = cValues.length

        for valueIndex in range(0, cValues.length):
            cValue = cValues.values[valueIndex]
            value = Task.fromCValue(cValue)
            values.append(value)

        return values

    @staticmethod
    def fromCValue(cValue):
        if cValue.type == CValue.Int32:
            return int(cValue.value.int32Value)
        elif cValue.type == CValue.String:
            return str(cValue.value.stringValue)

        return None

    @staticmethod
    def toCValues(values):
        length = len(values);

        innerValues = (CValue * length)()
        for index in range(0, length):
            innerValues[index] = Task.toCValue(values[index])

        cvalues = CValues()
        cvalues.length = length
        cvalues.values = pointer(innerValues[0])
        
        return cvalues

    @staticmethod
    def toCValue(value):
        cvalue = CValue()
        if type(value) == int:
            cvalue.type = CValue.Int32
            cvalue.value.int32Value = value
        elif type(value) == str:
            length = len(value)

            cvalue.type = CValue.String
            cvalue.length = length
            cvalue.value.stringValue = c_char_p(value)

        return cvalue

    def clone(self):
        raise NotImplemented

    def emit(self, values):
        cValues = Task.toCValues(values)
        self.emitter(self.wrapper, pointer(cValues))

class Spout(Task):
    OpenHandler = CFUNCTYPE(c_void_p, c_int)
    CloseHandler = CFUNCTYPE(c_void_p, c_int)
    ExecuteHandler = CFUNCTYPE(c_void_p, 
            c_int, c_void_p, Task.Emitter)

    def open(self):
        raise NotImplemented

    def close(self):
        raise NotImplemented

    def execute(self):
        raise NotImplemented

    def emit(self, values):
        cValues = Task.toCValues(values)
        self.emitter(self.wrapper, pointer(cValues))

    def getFields(self):
        raise NotImplemented

class CSpout(Structure):
    _fields_ = [('onClone', Spout.CloneHandler),
            ('onOpen', Spout.OpenHandler),
            ('onClose', Spout.CloseHandler),
            ('onExecute', Spout.ExecuteHandler)]

class Bolt(Task):
    PrepareHandler = CFUNCTYPE(c_void_p, c_int)
    CleanupHandler = CFUNCTYPE(c_void_p, c_int)
    ExecuteHandler = CFUNCTYPE(c_void_p, 
            c_int, c_void_p, Task.Emitter, POINTER(CValues))

    def open(self):
        raise NotImplemented

    def close(self):
        raise NotImplemented

    def execute(self, values):
        raise NotImplemented

    def getFields(self):
        raise NotImplemented

class CBolt(Structure):
    _fields_ = [('onClone', Bolt.CloneHandler),
            ('onPrepare', Bolt.PrepareHandler),
            ('onCleanup', Bolt.CleanupHandler),
            ('onExecute', Bolt.ExecuteHandler)]

class CTopology(Structure):
    _fields_ = [('cBolts', POINTER(CBolt)),
        ('boltNames', POINTER(c_char_p)),
        ('boltSource', POINTER(c_char_p)),
        ('boltCount', c_int),
        ('cSpouts', POINTER(CSpout)),
        ('spoutNames', POINTER(c_char_p)),
        ('spoutCount', c_int)]

class Topology(object):
    def __init__(self):
        self.spoutIndex = 0
        self.spoutNames = []
        self.spouts = []
        self.boltIndex = 0
        self.bolts = []
        self.boltNames = []
        self.boltSources = []

    def setSpout(self, spoutName, spout):
        self.createCSpout(spout)
        self.spoutNames.append(spoutName)

    def setBolt(self, boltName, bolt, src):
        self.createCBolt(bolt)
        self.boltSources.append(src)
        self.boltNames.append(boltName)

    def start(self):
        cTopology = CTopology()

        boltCount = self.boltIndex
        boltNames = (c_char_p * boltCount)()
        boltSources = (c_char_p * boltCount)()
        cBolts = (CBolt * boltCount)()
        for boltIndex in range(0, boltCount):
            boltNames[boltIndex] = c_char_p(self.boltNames[boltIndex])
            boltSources[boltIndex] = c_char_p(self.boltSources[boltIndex])
            cBolts[boltIndex] = self.bolts[boltIndex]

        spoutCount = self.spoutIndex
        spoutNames = (c_char_p * spoutCount)()
        cSpouts = (CSpout * spoutCount)()
        for spoutIndex in range(0, spoutCount):
            spoutNames[spoutIndex] = c_char_p(self.spoutNames[spoutIndex])
            cSpouts[boltIndex] = self.spouts[spoutIndex]

        cTopology.boltCount = boltCount
        cTopology.cBolts = cBolts
        cTopology.boltNames = boltNames
        cTopology.boltSources = boltSources
        cTopology.spoutCount = spoutCount
        cTopology.cSpouts = cSpouts
        cTopology.spoutNames = spoutNames

        pyHurricaneHandle.StartTopology(pointer(cTopology))

    def createCSpout(self, spout):
        def clone():
            self.spoutIndex = self.spoutIndex + 1 
            self.spouts.append(spout.clone())

            return self.spoutIndex - 1

        def open(spoutIndex):
            self.spouts[spoutIndex].open()

        def close(spoutIndex):
            self.spouts[spoutIndex].close()

        def execute(spoutIndex, wrapper, emitter):
            self.spouts[spoutIndex].wrapper = wrapper
            self.spouts[spoutIndex].emitter = emitter
            self.spouts[spoutIndex].execute()

        cSpout = CSpout()
        cSpout.onClone = Task.CloneHandler(clone)
        cSpout.onOpen = Spout.OpenHandler(open)
        cSpout.onClose = Spout.CloseHandler(close)
        cSpout.onExecute = Spout.ExecuteHandler(execute)

        return cSpout

    def createCBolt(self, bolt):
        def clone():
            self.boltIndex = self.boltIndex + 1 
            self.bolts.append(bolt.clone())

            return self.boltIndex - 1

        def prepare(boltIndex):
            self.bolts[boltIndex].prepare()

        def cleanup(boltIndex):
            self.bolts[boltIndex].cleanup()

        def execute(boltIndex, wrapper, emitter, cValues):
            values = Task.fromCValues(cValues.contents)

            self.bolts[boltIndex].wrapper = wrapper
            self.bolts[boltIndex].emitter = emitter
            self.bolts[boltIndex].execute(values)

        cBolt = CBolt()
        cBolt.onClone = Task.CloneHandler(clone)
        cBolt.onPrepare = Bolt.PrepareHandler(prepare)
        cBolt.onCleanup = Bolt.CleanupHandler(cleanup)
        cBolt.onExecute = Bolt.ExecuteHandler(execute)

        return cBolt

def main(cSpout, cBolt):
    pyHurricaneHandle.TestCSpout(pointer(cSpout))
    pyHurricaneHandle.TestCBolt(pointer(cBolt))
