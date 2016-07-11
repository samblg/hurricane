from util import FileUtil, Path, PathUtil
import os

class Document(object):
    def __init__(self, documentPath):
        self.path = documentPath
        self.rules = []
        self.subDocuments = []
        pass

    def __str__(self):
        return "GNU Makefile Document Object"

    def getPath(self):
        return self.path

    def addRule(self, rule):
        self.rules.append(rule)

    def addSubDocument(self, subDocument):
        self.subDocuments.append(subDocument)

    def writeToFile(self):
        makefileContent = self.toText()
        makefile = FileUtil.openFile(self.path)
        makefile.writelines(makefileContent)

        for subDocument in self.subDocuments:
            subDocument.writeToFile()

        makefile.close()

    def toText(self):
        makefileContent = []
        for rule in self.rules:
            makefileContent.append(rule.toText())
            makefileContent.append('')
        
        for subDocument in self.subDocuments:
            includeInstruction = "-include %(path)s" % { "path": subDocument.getPath() }
            makefileContent.append(includeInstruction)

        return '\n'.join(makefileContent)

class Rule(object):
    def __init__(self):
        self.prebuildActions = []
        self.buildActions = []
        self.postbuildActions = []
        self.target = ""
        pass

    def __str__(self):
        return "GNU Makefile Rule Object"

    def addPrebuildAction(self, action):
        self.prebuildActions.append(action)

    def addBuildAction(self, action):
        self.buildActions.append(action)

    def addPostbuildAction(self, action):
        self.postbuildActions.append(action)

    def setTarget(self, target):
        self.target = target

    def getTarget(self):
        return self.target

    def toText(self):
        textContent = []
        textContent.append(self.getTargetLine())
        
        for prebuildAction in self.prebuildActions:
            textContent.append('\t' +prebuildAction.toText())

        for buildAction in self.buildActions:
            textContent.append('\t' + buildAction.toText())

        for postbuildAction in self.postbuildActions:
            textContent.append('\t' + postbuildAction.toText())

        return '\n'.join(textContent)

    def getTargetLine(self):
        raise NotImplementedError()

class Action(object):
    def __init__(self):
        pass

    def __str__(self):
        return "GNU Makefile Action Object"

    def toText(self):
        raise NotImplementedError()

class SimpleRule(Rule):
    def __init__(self, target, prerequisites):
        Rule.__init__(self)
        self.setTarget(target)
        self.setPrerequisites(prerequisites)
        pass

    def setPrerequisites(self, prerequisites):
        self.prerequisites = prerequisites

    def getPrerequisites(self):
        return self.prerequisites

    def getTargetLine(self):
        shellStrings = []
        for prerequisite in self.prerequisites:
            shellStrings.append(prerequisite.shellString())

        return '%(target)s: %(prerequisites)s' % {
                'target': self.getTarget(),
                'prerequisites': ' '.join(shellStrings)
        }

class GCCCompileRule(Rule):
    def __init__(self, sourceFilePath, includePaths, sourceDirPath, objectDirPath, makeConf):
        Rule.__init__(self)

        relPath = sourceFilePath.getRelevantPath(sourceDirPath)
        dirName = relPath.getDirName()
        baseName = relPath.getBaseName()

        objectSubDirPath = objectDirPath.join(dirName)
        objectFilePath = objectSubDirPath.join(PathUtil.getPrefix(baseName))
        if sourceFilePath.getExt() not in makeConf['asm_src_exts']:
            objectFilePath.appendExt('o')
        if sourceFilePath.getExt() in makeConf['asm_src_exts'] and makeConf['asm_with_object_ext']:
            objectFilePath.appendExt('o')
        self.setTarget(objectFilePath.shellString())

        self.sourceFilePath = sourceFilePath
        self.includePaths = includePaths
        self.objectSubDirPath = objectSubDirPath

        if self.sourceFilePath.getExt() in makeConf['c_src_exts']:
            self.includePaths.extend(PathUtil.toPathList(makeConf['c_include_paths']))
        elif self.sourceFilePath.getExt() in makeConf['cxx_src_exts']:
            if makeConf['cxx_using_c_include_paths']:
                self.includePaths.extend(PathUtil.toPathList(makeConf['c_include_paths']))
            self.includePaths.extend(PathUtil.toPathList(makeConf['cxx_include_paths']))
        elif self.sourceFilePath.getExt() in makeConf['asm_src_exts']:
            self.includePaths.extend(PathUtil.toPathList(makeConf['asm_include_paths']))

        self.targetLine = self.generateTargetLine(makeConf)

        makeDirectoryAction = MakeDirectoryAction(objectSubDirPath)
        compileAction = GCCCompileAction(objectFilePath, sourceFilePath, includePaths, makeConf)
        self.addBuildAction(makeDirectoryAction)
        self.addBuildAction(compileAction)

    def generateTargetLine(self, makeConf):
        includePathShellStrings = []
        for includePath in self.includePaths:
            includePathShellStrings.append('-I' + includePath.shellString() + '/')

        dependChecker = ''
        compilerFlags = ''
        if self.sourceFilePath.getExt() in makeConf['c_src_exts']:
            dependChecker = makeConf['cc']
            compilerFlags = makeConf['cflags']
        elif self.sourceFilePath.getExt() in makeConf['cxx_src_exts']:
            dependChecker = makeConf['cxxc']
            compilerFlags = makeConf['cxxflags']
        elif self.sourceFilePath.getExt() in makeConf['asm_src_exts']:
            dependChecker = makeConf['as']
            compilerFlags = makeConf['asflags']

        dependOption = ''
        if self.sourceFilePath.getExt() in makeConf['c_src_exts']:
            dependOption = '-MM'
        elif self.sourceFilePath.getExt() in makeConf['cxx_src_exts']:
            dependOption = '-MM'
        elif self.sourceFilePath.getExt() in makeConf['asm_src_exts']:
            dependOption = '-M'

        gccCheckCommand = ('%(depend_checker)s %(compiler_flags)s %(depend_option)s %(filename)s %(include_main_path)s' %
                { 
                    'depend_checker': dependChecker,
                    'compiler_flags': compilerFlags,
                    'depend_option': dependOption,
                    'filename': self.sourceFilePath,
                    'include_main_path': ' '.join(includePathShellStrings)
                })
        gccCheckResult = os.popen(gccCheckCommand).readlines()

        targetLineContent = []
        if len(gccCheckResult) > 0:
            if self.sourceFilePath.getExt() in makeConf['c_src_exts'] or self.sourceFilePath.getExt() in makeConf['cxx_src_exts']:
                gccCheckResult[0] = str(self.objectSubDirPath.join(gccCheckResult[0]))
            elif self.sourceFilePath.getExt() in makeConf['asm_src_exts']:
                gccCheckResult[0] = str(self.getTarget() + gccCheckResult[0])

        for line in gccCheckResult:
            outputLine = str(line).rstrip()
            if len(outputLine) == 0 :
                continue
            if outputLine[-1] == '/':
                outputLine = outputLine[:-1] + '\\'
            targetLineContent.append(outputLine)

        return '\n'.join(targetLineContent)

    def getTargetLine(self):
        return self.targetLine

class GCCCompileAction(Action):
    def __init__(self, objectFilePath, sourceFilePath, includePaths, makeConf):
        Action.__init__(self)
        self.objectFilePath = objectFilePath
        self.sourceFilePath = sourceFilePath
        self.includePaths = includePaths
        self.makeConf = makeConf
        pass

    def toText(self):
        includePathShellStrings = []
        for includePath in self.includePaths:
            includePathShellStrings.append('-I' + includePath.shellString() + '/')

        compiler = ''
        compilerOption = ''
        compilerFlags = ''
        if self.sourceFilePath.getExt() in self.makeConf['c_src_exts']:
            compiler = self.makeConf['cc']
            compilerOption = '-c'
            compilerFlags = self.makeConf['cflags']
        elif self.sourceFilePath.getExt() in self.makeConf['cxx_src_exts']:
            compiler = self.makeConf['cxxc']
            compilerOption = '-c'
            compilerFlags = self.makeConf['cxxflags']
        elif self.sourceFilePath.getExt() in self.makeConf['asm_src_exts']:
            compiler = self.makeConf['as']
            compilerOption = ''
            compilerFlags = self.makeConf['asflags']

        dynamic_cflags = ''
        if self.makeConf['target_type'] == 'dynamic_library' and self.makeConf['fpic']:
            dynamic_cflags = ' -fPIC'

        compileCommand = ('%(compiler)s %(cflags)s %(compiler_option)s -o %(objectfile)s %(include_main_path)s %(srcfile)s' %
                { 'include_main_path': ' '.join(includePathShellStrings),
                    'compiler': compiler, 
                    'cflags': compilerFlags + dynamic_cflags,
                    'compiler_option': compilerOption, 
                    'objectfile': self.objectFilePath.shellString(), 
                    'srcfile': self.sourceFilePath.shellString() })

        return compileCommand

class GCCLinkRule(SimpleRule):
    def __init__(self, targetFilePath, objectFilePaths, makeConf):
        SimpleRule.__init__(self, targetFilePath, objectFilePaths)
        linkAction = GCCLinkAction(targetFilePath, objectFilePaths, makeConf)
        self.addBuildAction(linkAction)

class GCCLinkAction(Action):
    def __init__(self, targetFilePath, objectFilePaths, makeConf):
        Action.__init__(self)
        self.targetFilePath = targetFilePath
        self.objectFilePaths = objectFilePaths
        self.makeConf = makeConf
        pass

    def toText(self):
        shellStrings = []
        for objectFilePath in self.objectFilePaths:
            shellStrings.append(objectFilePath.shellString())

        libraries = self.makeConf['libraries']
        libraryFlags = []
        for library in libraries:
            libraryFlags.append('-l' + library)

        libraryPaths = self.makeConf['ld_library_paths']
        libraryPathFlags = []
        for libraryPath in libraryPaths:
            libraryPathFlags.append('-L' + libraryPath)
            
        dynamic_ldflags = ''
        if self.makeConf['target_type'] == 'dynamic_library':
            dynamic_ldflags = ' -shared'

        linkCommand = ('%(linker)s %(ldflags)s -o %(target_file)s %(object_files)s %(library_path_flags)s %(library_flags)s' % { 
                'final_file': self.targetFilePath.shellString(), 
                'object_files': ' '.join(shellStrings),
                'linker': self.makeConf['ld'],
                'ldflags': self.makeConf['ldflags'] + dynamic_ldflags,
                'target_file': self.targetFilePath.shellString(),
                'library_path_flags': ' '.join(libraryPathFlags),
                'library_flags': ' '.join(libraryFlags)
        })

        return linkCommand

class ArRule(SimpleRule):
    def __init__(self, targetFilePath, objectFilePaths, makeConf):
        SimpleRule.__init__(self, targetFilePath, objectFilePaths)
        arAction = ArAction(targetFilePath, objectFilePaths, makeConf)
        self.addBuildAction(arAction)

class ArAction(Action):
    def __init__(self, targetFilePath, objectFilePaths, makeConf):
        Action.__init__(self)
        self.targetFilePath = targetFilePath
        self.objectFilePaths = objectFilePaths
        self.makeConf = makeConf
        pass

    def toText(self):
        shellStrings = []
        for objectFilePath in self.objectFilePaths:
            shellStrings.append(objectFilePath.shellString())

        arCommand = ('%(ar)s cr %(target_file)s %(object_files)s' % { 
                'ar': self.makeConf['ar'],
                'target_file': self.targetFilePath.shellString(),
                'object_files': ' '.join(shellStrings)
        })

        return arCommand

class MakeDirectoryAction(Action):
    def __init__(self, directoryPath):
        Action.__init__(self)
        self.directoryPath = directoryPath
        pass

    def toText(self):
        mkDirCommand = 'mkdir -p %(directory_path)s' % {
                'directory_path': self.directoryPath.shellString()
        }

        return mkDirCommand

