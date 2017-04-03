#! /usr/bin/python3

from unittest import TestLoader, TextTestRunner, TestSuite
import os.path
import test.util

class AutomaticTestRunner:
    def __init__(self):
        self.loader = TestLoader()
        self.runner = TextTestRunner()

    def runTests(self):
        testCaseNames = self.findTestCaseNames()
        testCaseModules = self.loadTestCaseModules(testCaseNames)
        testSuite = self.loadTestsFromModules(testCaseModules)
        return self.runner.run(testSuite)

    def findTestCaseNames(self):
        rootSuite = __import__('test')
        rootSuitePath = rootSuite.__path__[0]
        return self.findTestCaseNamesFromDirectory(rootSuitePath)
    
    def loadTestCaseModules(self, testCaseNames):
        testCaseModules = []
        for testCaseName in testCaseNames:
            testCaseModule = __import__(testCaseName, {}, {}, ['a'])
            testCaseModules.append(testCaseModule)

        return testCaseModules

    def loadTestsFromModules(self, testCaseModules):
        suite = TestSuite()
        for testCaseModule in testCaseModules:
            subSuite = self.loader.loadTestsFromModule(testCaseModule)
            suite.addTest(subSuite)

        return suite

    def findTestCaseNamesFromDirectory(self, rootPath):
        subFiles = os.listdir(rootPath)

        testModuleNames = self.findSubModuleNames(rootPath, subFiles)
        subSuitePaths = self.findSubSuitePaths(rootPath, subFiles)
        subSuiteTestModuleNames = self.findTestCaseNamesFromSubSuitePaths(subSuitePaths)
        testModuleNames.extend(subSuiteTestModuleNames)

        return testModuleNames

    def findSubModuleNames(self, rootPath, subFiles):
        testModuleNames = []
        for subFile in subFiles:
            subFilePath = rootPath + '/' + subFile
            if os.path.isfile(subFilePath) and subFile.endswith('.py') and not subFile.startswith('__'):
                rootModuleName = os.path.relpath(rootPath).replace('/', '.')
                testModuleName = rootModuleName + '.' + subFile[0:-3]
                testModuleNames.append(testModuleName)

        return testModuleNames

    def findSubSuitePaths(self, rootPath, subFiles):
        subSuitePaths = []
        for subFile in subFiles:
            subFilePath = rootPath + '/' + subFile
            if os.path.isdir(subFilePath) and not subFile.startswith('__'):
                subSuitePaths.append(subFilePath)

        return subSuitePaths

    def findTestCaseNamesFromSubSuitePaths(self, subSuitePaths):
        testModuleNames = []

        for subSuitePath in subSuitePaths:
            subTestModuleNames = self.findTestCaseNamesFromDirectory(subSuitePath)
            testModuleNames.extend(subTestModuleNames)

        return testModuleNames

def main():
    testRunner = AutomaticTestRunner()
    testResult = testRunner.runTests()

if __name__ == '__main__':
    main()
