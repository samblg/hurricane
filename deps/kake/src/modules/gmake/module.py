import os
import sys
import modules.module

from util import Path, PathUtil, FileUtil, Configuration, ConsoleLogger
from backends.gmake.engine import Document as GMakeDocument
from backends.gmake.engine import SimpleRule as GMakeSimpleRule
from backends.gmake.engine import GCCCompileRule
from backends.gmake.engine import GCCLinkRule
from backends.gmake.engine import ArRule

SRC_PATH = Path('src', True)
INCLUDE_PATH = Path('include', True)
TARGET_PATH = Path('target', True)
SRC_MAIN_PATH = SRC_PATH.join('main')
SRC_TEST_PATH = SRC_PATH.join('test')
INCLUDE_MAIN_PATH = INCLUDE_PATH.join('main')
INCLUDE_TEST_PATH = INCLUDE_PATH.join('test')
TARGET_OBJECT_PATH = TARGET_PATH.join('object')
TARGET_OBJECT_MAIN_PATH = TARGET_OBJECT_PATH.join('main')
TARGET_SUBMAKE_PATH = TARGET_PATH.join('submake')
TARGET_SUBMAKE_MAIN_PATH = TARGET_SUBMAKE_PATH.join('main')
TARGET_MAKEFILE_PATH = TARGET_PATH.join('Makefile')
FINAL_TARGET_SUBMAKE_PATH = TARGET_SUBMAKE_PATH.join('final_target.mk')

class Module(modules.module.Module):
    mainSourceFiles = []
    objectFiles = []
    defaultConf = {}

    def __init__(self):
        self.defaultConf['target_type'] = 'executable'
        self.defaultConf['cc'] = 'gcc'
        self.defaultConf['cxxc'] = 'g++'
        self.defaultConf['cflags'] = ''
        self.defaultConf['cxxflags'] = ''
        self.defaultConf['fpic'] = True
        self.defaultConf['autolink'] = True
        self.defaultConf['ar'] = 'ar'
        self.defaultConf['ld'] = 'g++'
        self.defaultConf['ldflags'] = ''
        self.defaultConf['ld_library_paths'] = []
        self.defaultConf['c_src_exts'] = ['.c']
        self.defaultConf['cxx_src_exts'] = ['.cpp']
        self.defaultConf['c_include_paths'] = []
        self.defaultConf['cxx_include_paths'] = []
        self.defaultConf['cxx_using_c_include_paths'] = True
        self.logger = ConsoleLogger('cmodule')

    def initKakefile(self):
        kakefile = FileUtil.openFile('Kakefile')
        kakefileContent = """project:
    name: ${name}
    version: ${version}
    type: cpp"""
        kakefile.write(kakefileContent)
        kakefile.close()

        return True

    def init(self):
        FileUtil.createDirectory(SRC_PATH)
        FileUtil.createDirectory(INCLUDE_PATH)
        FileUtil.createDirectory(TARGET_PATH)
        FileUtil.createDirectory(SRC_MAIN_PATH)
        FileUtil.createDirectory(SRC_TEST_PATH)
        FileUtil.createDirectory(INCLUDE_MAIN_PATH)
        FileUtil.createDirectory(INCLUDE_TEST_PATH)

        return True

    def make(self, projectConfig):
        makeConf = {
            'target_type': projectConfig.getItem('make.configuration.target.type', self.defaultConf['target_type']),
            'cc': projectConfig.getItem('make.configuration.compiler.c.cc', self.defaultConf['cc']),
            'cxxc': projectConfig.getItem('make.configuration.compiler.cpp.cc', self.defaultConf['cxxc']),
            'cflags': projectConfig.getItem('make.configuration.compiler.c.flags', self.defaultConf['cflags']),
            'cxxflags': projectConfig.getItem('make.configuration.compiler.cpp.flags', self.defaultConf['cxxflags']),
            'fpic': projectConfig.getItem('make.configuration.compiler.fpic', self.defaultConf['fpic']),
            'autolink': projectConfig.getItem('make.configuration.linker.autolink', self.defaultConf['autolink']),
            'ar': projectConfig.getItem('make.configuration.archiver.ar', self.defaultConf['ar']),
            'ld': projectConfig.getItem('make.configuration.linker.ld', self.defaultConf['ld']),
            'ldflags': projectConfig.getItem('make.configuration.linker.flags', self.defaultConf['ldflags']),
            'ld_library_paths': projectConfig.getItem('make.configuration.linker.library_paths', self.defaultConf['ld_library_paths']),
            'libraries': projectConfig.getItem('make.configuration.linker.libraries', []),
            'c_src_exts': projectConfig.getItem('make.configuration.compiler.c.src_exts', self.defaultConf['c_src_exts']),
            'cxx_src_exts': projectConfig.getItem('make.configuration.compiler.cpp.src_exts', self.defaultConf['cxx_src_exts']),
            'c_include_paths': projectConfig.getItem('make.configuration.compiler.c.include_paths', self.defaultConf['c_include_paths']),
            'cxx_include_paths': projectConfig.getItem('make.configuration.compiler.cpp.include_paths', self.defaultConf['cxx_include_paths']),
            'cxx_using_c_include_paths': projectConfig.getItem('make.configuration.compiler.cpp.inherit_c_include_path', self.defaultConf['cxx_using_c_include_paths'])
        }

        makefile = GMakeDocument(TARGET_MAKEFILE_PATH)

        self.mainSourceFiles.clear()
        self.objectFiles.clear()
        cSourceFiles = []
        cppSourceFiles = []
        FileUtil.searchAllFiles(cSourceFiles, SRC_MAIN_PATH, makeConf['c_src_exts'])
        FileUtil.searchAllFiles(cppSourceFiles, SRC_MAIN_PATH, makeConf['cxx_src_exts'])
        self.mainSourceFiles.extend(cSourceFiles)
        self.mainSourceFiles.extend(cppSourceFiles)

        for fileName in self.mainSourceFiles:
            filePath = Path(fileName, True)
            relPath = filePath.getRelevantPath(SRC_MAIN_PATH)
            dirName = relPath.getDirName()
            baseName = relPath.getBaseName()

            subMakeDirPath = TARGET_SUBMAKE_MAIN_PATH.join(dirName)
            FileUtil.createDirectory(subMakeDirPath)

            subMakefilePath = subMakeDirPath.join(PathUtil.getPrefix(baseName))
            subMakefilePath.appendExt('mk')
            subMakefile = GMakeDocument(subMakefilePath)

            compileRule = GCCCompileRule(filePath, [INCLUDE_MAIN_PATH], SRC_MAIN_PATH, TARGET_OBJECT_MAIN_PATH, makeConf)
            subMakefile.addRule(compileRule)
            makefile.addSubDocument(subMakefile)

            objectFilePath = Path(compileRule.getTarget())
            self.objectFiles.append(objectFilePath)

        if makeConf['autolink']:
            if makeConf['target_type'] == 'executable':
                subMakefile = GMakeDocument(FINAL_TARGET_SUBMAKE_PATH)
                
                finalFileName = '%(name)s-%(version)s' % { 
                        'name': projectConfig.getItem('project.name', 'noname'),
                        'version': projectConfig.getItem('project.version', '1.0.0')}
                finalFilePath = TARGET_PATH.join(finalFileName)

                linkRule = GCCLinkRule(finalFilePath, self.objectFiles, makeConf)
                subMakefile.addRule(linkRule)

                makefile.addSubDocument(subMakefile)

                allRule = GMakeSimpleRule('all', [finalFilePath])
                makefile.addRule(allRule)
            elif makeConf['target_type'] == 'dynamic_library':
                subMakefile = GMakeDocument(FINAL_TARGET_SUBMAKE_PATH)
                
                finalFileName = 'lib%(name)s.so.%(version)s' % { 
                        'name': projectConfig.getItem('project.name', 'noname'),
                        'version': projectConfig.getItem('project.version', '1.0.0')}
                finalFilePath = TARGET_PATH.join(finalFileName)

                linkRule = GCCLinkRule(finalFilePath, self.objectFiles, makeConf)
                subMakefile.addRule(linkRule)

                makefile.addSubDocument(subMakefile)

                allRule = GMakeSimpleRule('all', [finalFilePath])
                makefile.addRule(allRule)
            elif makeConf['target_type'] == 'static_library':
                subMakefile = GMakeDocument(FINAL_TARGET_SUBMAKE_PATH)
                
                finalFileName = 'lib%(name)s.a.%(version)s' % { 
                        'name': projectConfig.getItem('project.name', 'noname'),
                        'version': projectConfig.getItem('project.version', '1.0.0')}
                finalFilePath = TARGET_PATH.join(finalFileName)

                arRule = ArRule(finalFilePath, self.objectFiles, makeConf)
                subMakefile.addRule(arRule)

                makefile.addSubDocument(subMakefile)

                allRule = GMakeSimpleRule('all', [finalFilePath])
                makefile.addRule(allRule)
            else:
                self.logger.warn('target_type is not recognized!')
                sys.exit(1)
        else:
            allRule = GMakeSimpleRule('all', self.objectFiles)
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
        for cleanPath in (TARGET_OBJECT_PATH, 
                TARGET_OBJECT_MAIN_PATH, 
                TARGET_SUBMAKE_PATH, 
                TARGET_SUBMAKE_MAIN_PATH,
                TARGET_MAKEFILE_PATH):
            self.logger.debug('Remove: ' + str(cleanPath))
            os.system('rm -rf ' + cleanPath.shellString())

        return True
