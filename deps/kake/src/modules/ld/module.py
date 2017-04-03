import os
import modules.module

from util import Path, PathUtil, FileUtil, Configuration, ConsoleLogger
from backends.gmake.engine import Document as GMakeDocument
from backends.gmake.engine import SimpleRule as GMakeSimpleRule
from backends.gmake.engine import GCCCompileRule
from backends.gmake.engine import GCCLinkRule
from backends.gmake.engine import ArRule

TARGET_PATH = Path('target', True)
TARGET_MAKEFILE_PATH = TARGET_PATH.join('Makefile')
TARGET_SUBMAKE_PATH = TARGET_PATH.join('submake')
FINAL_TARGET_SUBMAKE_PATH = TARGET_SUBMAKE_PATH.join('final_target')

class Module(modules.module.Module):
    objectFiles = []
    defaultConf = {}

    def __init__(self):
        modules.module.Module.__init__(self)
        self.defaultConf['target_type'] = 'executable'
        self.defaultConf['ar'] = 'ar'
        self.defaultConf['ld'] = 'g++'
        self.defaultConf['ldflags'] = ''
        self.defaultConf['ld_library_paths'] = []
        self.logger = ConsoleLogger('ldmodule')

    def initKakefile(self):
        kakefile = FileUtil.openFile('Kakefile')
        kakefileContent = """project:
    name: ${name}
    version: ${version}
    type: ld
    
    make:
        configuration:
            linker:
                include: [ ]"""
        kakefile.write(kakefileContent)
        kakefile.close()
        
        return True

    def init(self):
        FileUtil.createDirectory(TARGET_PATH)

        return True

    def make(self, projectConfig):
        makeConf = {
            'target_type': projectConfig.getItem('make.configuration.target.type', self.defaultConf['target_type']),
            'ar': projectConfig.getItem('make.configuration.archiver.ar', self.defaultConf['ar']),
            'ld': projectConfig.getItem('make.configuration.linker.ld', self.defaultConf['ld']),
            'ldflags': projectConfig.getItem('make.configuration.linker.flags', self.defaultConf['ldflags']),
            'ld_library_paths': projectConfig.getItem('make.configuration.linker.library_paths', self.defaultConf['ld_library_paths']),
            'libraries': projectConfig.getItem('make.configuration.linker.libraries', []),
            'solution_dir': projectConfig.getItem('global.solution.dir', '..'),
            'sources': projectConfig.getItem('make.configuration.linker.include', []),
        }

        makefile = GMakeDocument(TARGET_MAKEFILE_PATH)
        objectSourcePaths = []

        self.objectFiles.clear()
        solutionDirPath = Path(makeConf['solution_dir'], True)
        objectFilePaths = []
        for objectSourcePath in makeConf['sources']:
            objectSourcePath = solutionDirPath.join(objectSourcePath)
            FileUtil.searchAllFiles(objectFilePaths, objectSourcePath, [ '.o' ])

        for objectFilePath in objectFilePaths:
            self.objectFiles.append(Path(objectFilePath))

        finalFilePath = self.getFinalFilePath(projectConfig, makeConf)
        linkRule = self.getLinkRule(finalFilePath, makeConf)
        subMakefile = GMakeDocument(FINAL_TARGET_SUBMAKE_PATH)
        subMakefile.addRule(linkRule)

        makefile.addSubDocument(subMakefile)

        allRule = GMakeSimpleRule('all', [finalFilePath])
        makefile.addRule(allRule)

        makefile.writeToFile()
        
        return True

    def build(self, projectConfig):
        returnValue = os.system('make -f %(makefile)s' % {
            'makefile': TARGET_PATH.join('Makefile').shellString() })

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
        makeConf = self.getMakeConf(projectConfig)
        finalFilePath = self.getFinalFilePath(projectConfig, makeConf)

        for cleanPath in (TARGET_SUBMAKE_PATH, 
                TARGET_MAKEFILE_PATH,
                FINAL_TARGET_SUBMAKE_PATH,
                finalFilePath):
            self.logger.debug('Remove: ' + str(cleanPath))
            os.system('rm -rf ' + cleanPath.shellString())

        return True

    def getMakeConf(self, projectConfig):
        return {
            'target_type': projectConfig.getItem('make.configuration.target.type', self.defaultConf['target_type']),
            'ar': projectConfig.getItem('make.configuration.archiver.ar', self.defaultConf['ar']),
            'ld': projectConfig.getItem('make.configuration.linker.ld', self.defaultConf['ld']),
            'ldflags': projectConfig.getItem('make.configuration.linker.flags', self.defaultConf['ldflags']),
            'ld_library_paths': projectConfig.getItem('make.configuration.linker.library_paths', self.defaultConf['ld_library_paths']),
            'libraries': projectConfig.getItem('make.configuration.linker.libraries', []),
            'solution_dir': projectConfig.getItem('global.solution.dir', '..'),
            'sources': projectConfig.getItem('make.configuration.linker.include', []),
        }


    def getFinalFilePath(self, projectConfig, makeConf):
        finalFileName = ''
        if makeConf['target_type'] == 'executable':
            finalFileName = '%(name)s-%(version)s' % { 
                    'name': projectConfig.getItem('project.name', 'noname'),
                    'version': projectConfig.getItem('project.version', '1.0.0')}
        elif makeConf['target_type'] == 'dynamic_library':
            finalFileName = 'lib%(name)s.so.%(version)s' % { 
                    'name': projectConfig.getItem('project.name', 'noname'),
                    'version': projectConfig.getItem('project.version', '1.0.0')}
        elif makeConf['target_type'] == 'static_library':
            finalFileName = 'lib%(name)s.a.%(version)s' % { 
                    'name': projectConfig.getItem('project.name', 'noname'),
                    'version': projectConfig.getItem('project.version', '1.0.0')}
        
        return TARGET_PATH.join(finalFileName)
    
    def getLinkRule(self, finalFilePath, makeConf):
        linkRule = None
        if makeConf['target_type'] == 'executable':
            linkRule = GCCLinkRule(finalFilePath, self.objectFiles, makeConf)
        elif makeConf['target_type'] == 'dynamic_library':
            linkRule = GCCLinkRule(finalFilePath, self.objectFiles, makeConf)
        elif makeConf['target_type'] == 'static_library':
            linkRule = ArRule(finalFilePath, self.objectFiles, makeConf)
        else:
            self.logger.warn('target_type is not recognized!')
            sys.exit(1)
        
        return linkRule
