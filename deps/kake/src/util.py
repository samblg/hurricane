import os
import sys
import logging

class ConsoleLogger(object):
    def __init__(self, name):
        handler = logging.StreamHandler(sys.stdout)
        fmt = '[%(levelname)s] %(message)s'
        formatter = logging.Formatter(fmt)
        handler.setFormatter(formatter)

        self.logger = logging.getLogger(name)
        self.logger.addHandler(handler)
        self.logger.setLevel(logging.DEBUG)

    def critical(self, msg, *args, **kwargs):
        self.logger.critical(msg, *args, **kwargs)

    def debug(self, msg, *args, **kwargs):
        self.logger.debug(msg, *args, **kwargs)

    def error(self, msg, *args, **kwargs):
        self.logger.error(msg, *args, **kwargs)

    def exception(self, msg, *args, **kwargs):
        self.logger.exception(msg, *args, **kwargs)

    def fatal(self, msg, *args, **kwargs):
        self.logger.fatal(msg, *args, **kwargs)

    def info(self, msg, *args, **kwargs):
        self.logger.info(msg, *args, **kwargs)

    def warn(self, msg, *args, **kwargs):
        self.logger.warn(msg, *args, **kwargs)

logger = ConsoleLogger('util')

class PathUtil(object):
    @classmethod
    def join(cls, path1, path2):
        return Path(path1).join(path2).string()

    @classmethod
    def getPrefix(cls, path):
        return Path(path).getPrefix()
    
    @classmethod
    def exist(cls, filePath):
        filename = str(filePath)
        return os.path.exists(str(filename))

    @classmethod
    def toPathList(cls, stringList, isAbsolutePath = False):
        pathList = []
        for string in stringList:
            pathList.append(Path(string, isAbsolutePath))
        
        return pathList

class FileUtil(object):
    @classmethod
    def openFile(cls, filePath):
        filePath = Path(filePath)
        dirName = filePath.getDirName()
        if dirName and not os.path.exists(dirName):
            os.makedirs(dirName)
        return open(filePath.string(), 'w')

    @classmethod
    def createDirectory(cls, dirPath):
        dirName = str(dirPath)
        if os.path.exists(dirName):
            logger.debug('Skip creating exists directory: %(dirName)s' % {'dirName': dirName})
        else:
            logger.debug('Create directory: %(dirName)s' % {'dirName': dirName})
            os.makedirs(dirName)

    @classmethod
    def searchAllFiles(cls, fileList, root, exts):
        for dirPath, dirNames, fileNames in os.walk(str(root), True):
            for fileName in fileNames:
                filePath = Path(fileName)
                if filePath.getExt() in exts:
                    fileList.append(PathUtil.join(dirPath, fileName))

class Path(object):
    path = ""
    absolute = False

    def __init__(self):
        self.path = ""
        self.absolute = False
        pass

    def __init__(self, path, isAbsolutePath = False):
        if isAbsolutePath:
            self.setAbsolutePath(path)
        else:
            self.setRelevantPath(path)

    def __str__(self):
        return self.string()

    def string(self):
        return self.path

    def shellString(self):
        return self.path.replace(' ', '\\ ')

    def clone(self):
        return Path(self.getPath(), self.absolute)

    def setAbsolutePath(self, path):
        self.path = os.path.abspath(str(path))
        self.absolute = True

    def setRelevantPath(self, path):
        self.path = str(path)
        self.absolute = False

    def getPath(self):
        return self.path

    def getRelevantPath(self, rootPath):
        return Path(os.path.relpath(self.path, str(rootPath)), False)

    def getDirName(self):
        return os.path.dirname(self.path)

    def getBaseName(self):
        return os.path.basename(self.path)

    def join(self, subPath):
        return self.clone().joinInPlace(subPath)

    def joinInPlace(self, subPath):
        if ( self.path.endswith('/') ):
            self.path = self.path + str(subPath)
        else:
            self.path = self.path + "/" + str(subPath)

        return self

    def getPrefix(self):
        return os.path.extsep.join(os.path.splitext(self.path)[:-1])

    def getExt(self):
        return os.path.splitext(self.path)[-1]

    def appendExt(self, ext):
        self.path = self.path + os.path.extsep + ext

    def removeExt(self, ext):
        self.path = self.getPrefix()

    def replaceExt(self, ext):
        self.path = self.getPrefix() + os.path.extsep + ext
        
class Configuration(object):
    def __init__(self):
        self.configuration = {}

    @classmethod
    def fromDictionary(cls, dictionary):
        configuration = Configuration()
        configuration.configuration = dictionary
        return configuration

    def toDictionary(self):
        return self.configuration

    def getItem(self, path, defaultValue = None):
        keys = path.split('.')
        currentValue = self.configuration
        try:
            for key in keys:
                currentValue = currentValue[key]
        except KeyError:
            currentValue = defaultValue

        return currentValue

    def setItem(self, path, value):
        keys = path.split('.')
        currentDictionary = self.configuration

        keyIndex = 0
        for key in keys:
            keyIndex = keyIndex + 1
            if not keyIndex == len(keys):
                if key not in currentDictionary:
                    currentDictionary[key] = {}
                currentDictionary = currentDictionary[key]
            else:
                currentDictionary[key] = value
