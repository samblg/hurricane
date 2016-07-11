import os
import sys
import modules.module

from util import Path, PathUtil, FileUtil, Configuration, ConsoleLogger
from backends.gmake.engine import Document as GMakeDocument
from backends.gmake.engine import SimpleRule as GMakeSimpleRule
from backends.gmake.engine import GCCCompileRule
from backends.gmake.engine import GCCLinkRule
from backends.gmake.engine import ArRule

PROJECT_PATH = Path('project', True)
TARGET_PATH = Path('target', True)

class Module(modules.module.Module):
    defaultConf = {}
    projects = []

    def __init__(self):
        #'solution_dir': projectConfig.getItem('global.solution.dir', '..'),
        self.logger = ConsoleLogger('cmodule')

        projectNames = os.listdir('project')
        projectNames.sort()
        for projectName in projectNames:
            project = {
                'name': projectName,
                'path': PROJECT_PATH.join(projectName)
            }
            self.projects.append(project)


    def initKakefile(self):
        kakefile = FileUtil.openFile('Kakefile')
        kakefileContent = """project:
    name: ${name}
    version: ${version}
    type: solution

run:
    default: ${default}"""
        kakefile.write(kakefileContent)
        kakefile.close()

        return True

    def init(self):
        FileUtil.createDirectory(PROJECT_PATH)
        FileUtil.createDirectory(TARGET_PATH)

        return True

    def make(self, projectConfig):
        return True

    def build(self, projectConfig):
        for project in self.projects:
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
            

        returnValue = 0
        if returnValue == 0:
            return True
        else:
            return False

    def test(self):
        pass

    def package(self):
        pass

    def deploy(self):
        pass

    def run(self, projectConfig):
        finalFileName = '%(name)s-%(version)s' % { 
                'name': projectConfig.getItem('project.name', 'noname'),
                'version': projectConfig.getItem('project.version', '1.0.0') }
        finalFilePath = TARGET_PATH.join(finalFileName)
        arguments = projectConfig.getItem("global.args", [])
        shellArguments = []
        for argument in arguments:
            shellArgument = '"' + argument + '"'
            shellArguments.append(shellArgument)

        runCommand = 'cd target && %(file_path)s %(arguments)s' % {
                'file_path': finalFilePath.shellString(),
                'arguments': ' '.join(shellArguments)
        }
        
        return os.system(runCommand)

    def clean(self, projectConfig):
        return True
