import os
import getopt
import sys
import yaml

from modules.module import isStandardPhase, isPhaseWithConfig, isPhaseWithoutConfig, ModuleManager
from util import Configuration, ConsoleLogger

DEFAULT_FILENAME = 'Kakefile'
WORK_PATH = '.'
logger = ConsoleLogger('main')

def usage():
    print(
    """Kake: An advanced make tool for KLUM
    -h --help       Show help
    -v --verbose    Verbose mode
    -f --filename=  Specify another project filename or will use default project filename 
    """)
    sys.exit()

def reportWrongArguments():
    logger.fatal('Error occurs when process arguments')
    sys.exit()

def reportNoProjectFile():
    logger.fatal('Project file not exists.')
    sys.exit()

def reportWrongPhase():
    logger.fatal('Phase is not standard phase.')
    sys.exit()

def existProjectFile(projectFilename):
    for filename in os.listdir(WORK_PATH):
        if os.path.isfile(filename) and projectFilename == filename:
            return True

    return False

def processArguments(globalConfig):
    try:
        options,args = getopt.getopt(sys.argv[1:],
            "hvf:", ["help", "verbose", "filename="])
    except getopt.GetoptError:
        reportWrongArguments()

    remainingArguments = args
    for name, value in options:
        if name in ("-h", "--help"):
            usage()
        if name in ("-v", "--verbose"):
            globalConfig.setItem('mode.verbose', True)
        if name in ("-f", "--filename"):
            globalConfig.setItem('project.filename', value)

    return remainingArguments

def kake(globalConfig, remainingArguments):
    makePhase = globalConfig.getItem('make.phase')
    if not isStandardPhase(makePhase):
        reportWrongPhase()

    moduleManager = ModuleManager()

    if isPhaseWithConfig(makePhase):
        projectFilename = globalConfig.getItem('project.filename')
        if existProjectFile(projectFilename) == False:
            reportNoProjectFile()

        projectFile = open(projectFilename)
        projectFileContent = projectFile.read()
        projectFile.close()
        projectConfig = yaml.load(projectFileContent)
        projectConfig['global'] = globalConfig.toDictionary()
        projectConfig['global']['args'] = remainingArguments

        module = moduleManager.getModule(projectConfig['project']['type'])
        moduleAction = moduleManager.getModuleAction(module, makePhase)
        moduleAction(Configuration.fromDictionary(projectConfig))
    elif isPhaseWithoutConfig(makePhase):
        module = moduleManager.getModule(remainingArguments[0])
        moduleAction = moduleManager.getModuleAction(module, makePhase)
        moduleAction()

def main():
    globalConfig = Configuration()
    globalConfig.setItem('mode.verbose', False)
    globalConfig.setItem('project.filename', DEFAULT_FILENAME)
    globalConfig.setItem('make.phase', 'build')

    remainingArguments = processArguments(globalConfig)

    if len(remainingArguments) > 0:
        globalConfig.setItem('make.phase', remainingArguments[0])
        remainingArguments = remainingArguments[1:]

    kake(globalConfig, remainingArguments)

if __name__ == "__main__":
    main()
