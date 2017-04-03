import os
import util
from util import Path
from util import PathUtil, FileUtil
from util import ConsoleLogger

FUNCTIONS_WITH_CONF = set(['make', 'build', 'run', 'clean'])
FUNCTIONS_WITHOUT_CONF = set(['init'])
logger = ConsoleLogger('module')

STANDARD_PHASE = set(['init', 'make', 'build', 'test', 'package', 'deploy', 'run', 'clean'])

def MakeBuildVersion():
    versionFile = None
    if not PathUtil.exist('target/BUILD_VERSION'):
        versionFile = FileUtil.openFile('target/BUILD_VERSION')
    else:
        versionFile = open('target/BUILD_VERSION', 'w')
    versionFile.close() 
    
def KakefileChanged():
    if PathUtil.exist('target/BUILD_VERSION'):
        kakefileModifiedTime = os.path.getmtime('Kakefile')
        lastBuildTime = os.path.getmtime('target/BUILD_VERSION')
        if kakefileModifiedTime > lastBuildTime:
            return True

    return False

class Module(object):
    def initCaller(self):
        self.enterPhase('Init')
        if not PathUtil.exist('Kakefile'):
            self.initKakefile()
        else:
            logger.debug('Skip initialize Kakefile')
        self.init()
        self.exitPhase('Init')

    def makeCaller(self, projectConfig):
        self.enterPhase('Make')
        isSuccessful = self.make(projectConfig)
        self.exitPhase('Make')

        return isSuccessful

    def buildCaller(self, projectConfig):
        if KakefileChanged():
            self.cleanCaller(projectConfig)

        if not self.makeCaller(projectConfig):
            return False

        self.enterPhase('Build')
        isSuccessful = self.build(projectConfig)
        MakeBuildVersion()
        self.exitPhase('Build')
        
        return isSuccessful

    def testCaller(self):
        self.enterPhase('Test')
        self.test()
        self.exitPhase('Test')

    def packageCaller(self):
        self.enterPhase('Package')
        self.package()
        self.exitPhase('Package')

    def deployCaller(self):
        self.enterPhase('Deployment')
        self.deploy()
        self.exitPhase('Deployment')

    def runCaller(self, projectConfig):
        if not self.buildCaller(projectConfig):
            return False

        self.enterPhase('Run')
        returnValue = self.run(projectConfig)
        self.exitPhase('Run')

        return returnValue

    def cleanCaller(self, projectConfig):
        self.enterPhase('Clean')
        self.clean(projectConfig)
        self.exitPhase('Clean')

    def enterPhase(self, phaseName):
        logger.info('Enter phase: ' + phaseName)

    def exitPhase(self, phaseName):
        logger.info('Exit phase: ' + phaseName)

    def initKakefile(self):
        raise NotImplementedError()
    def init(self):
        raise NotImplementedError()
    def make(self, projectConfig):
        raise NotImplementedError()
    def build(self, projectConfig):
        raise NotImplementedError()
    def test(self):
        raise NotImplementedError()
    def package(self):
        raise NotImplementedError()
    def deploy(self):
        raise NotImplementedError()
    def run(self, projectConfig):
        raise NotImplementedError()
    def clean(self, projectConfig):
        raise NotImplementedError()
        
def isStandardPhase(phase):
    return phase in STANDARD_PHASE

def isPhaseWithConfig(phase):
    return phase in FUNCTIONS_WITH_CONF

def isPhaseWithoutConfig(phase):
    return phase in FUNCTIONS_WITHOUT_CONF

class ModuleManager(object):
    moduleList = []

    def __init__(self):
        self.scanModules()

    def scanModules(self):
        modules = __import__('modules')
        modulesPath = modules.__path__[0]
        modulesSubFiles = os.listdir(modulesPath)
        for modulesSubFile in modulesSubFiles:
            if os.path.isdir(PathUtil.join(modulesPath, modulesSubFile)) and not modulesSubFile.startswith('__'):
                self.moduleList.append(modulesSubFile)
        
    def getModule(self, projectType):
        if not projectType in self.moduleList:
            self.reportProjectTypeNotSupported()

        moduleName = 'modules.' + projectType + '.module'
        moduleFile = __import__(moduleName, {}, {}, ['a'])
        module = moduleFile.Module()

        return module

    def reportProjectTypeNotSupported(self):
        logger.fatal('The project type is not supported')
        sys.exit()

    def getModuleAction(self, module, phase):
        if hasattr(module, phase):
            return getattr(module, phase + 'Caller')
        return None
