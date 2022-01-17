'use strict';

const uuid = require('uuid');
const path = require('path');
const util = {
    fs: require('../util/fs'),
    loggers: require('../util/loggers'),
    xml: require('../util/xml')
};
const fs = require('fs');
const logger = util.loggers.kake;

const XmlElement = util.xml.XmlElement;

class Solution {
    static create(options) {
        return new Solution(options);
    }

    constructor(options) {
        this._cache = options.cache;

        const guidKey = 'solution.guid';

        if ( !this._cache.options[guidKey] ) {
            this._cache.options[guidKey] = uuid.v4().toUpperCase()
        }

        this._guid = this._cache.options[guidKey];

        this._formatVersion = options.formatVersion;
        this._vsVersion = options.vsVersion;
        this._minVsVersion = options.minVsVersion;

        this._configurationPlatforms = [new ConfigruationIdentifier({
            configuration: 'Release',
            platform: 'x64'
        })];

        this._projects = [];

        if ( options.projects ) {
            this._projects = options.projects;
        }

        this.buildProjectTree();
        let nestedProjects = this.generateNestedProjects();

        this._globalSections = [];
        let solutionConfigurationPlatforms = 
            this._configurationPlatforms.map(configurationPlatform => {
                let configurationString = configurationPlatform.toString();
                return {
                    key: configurationString,
                    value: configurationString
                };
        });

        let projectConfigurationPlatforms = [];
        this._projects.forEach(project => {
            let configurationName = 'Release';
            let cpu = 'x64';

            let value = `${configurationName}|${cpu}`;
            let activeKey = `{${project._guid}}.${value}.ActiveCfg`;
            let buildKey = `{${project._guid}}.${value}.Build.0`;

            projectConfigurationPlatforms.push({
                key: activeKey,
                value: value
            });

            projectConfigurationPlatforms.push({
                key: buildKey,
                value: value
            });
        });

        this._globalSections.push(new SolutionGlobalSection({
            name: 'SolutionConfigurationPlatforms',
            type: 'preSolution',
            records: solutionConfigurationPlatforms
        }));
        
        this._globalSections.push(new SolutionGlobalSection({
            name: 'ProjectConfigurationPlatforms',
            type: 'postSolution',
            records: projectConfigurationPlatforms
        }));

        this._globalSections.push(new SolutionGlobalSection({
            name: 'NestedProjects',
            type: 'preSolution',
            records: nestedProjects
        }));
    }

    generateNestedProjects(currentNode) {
        if ( !currentNode ) {
            currentNode = this._projectTree;
        }

        let items = [];
        if ( currentNode.type === Solution.ProjectTypeGuid.Project ) {
            return items;
        }

        if ( currentNode.type === Solution.ProjectTypeGuid.Directory ) {
            this._projectDirectories.push(new SolutionDirectory({
                name: currentNode.name,
                guid: currentNode.guid
            }));
        }

        currentNode.items.forEach(item => {
            if ( currentNode.type === Solution.ProjectTypeGuid.Directory ||
              currentNode.type === 'root' ) {
                items = items.concat(this.generateNestedProjects(item));
            }

            if ( currentNode.type === 'root' ) {
                return;
            }

            items.push({
                key: `{${item.guid}}`,
                value: `{${currentNode.guid}}`
            });
        });

        return items;
    }

    buildProjectTree() {
        this._projectTree = {
            type: 'root',
            items: new Map()
        };
        this._projectDirectories = [];

        this._projects.forEach(project => {
            const pathParts = project._innerPath.split('/');
            let currentNode = this._projectTree;

            pathParts.forEach((pathPart, partIndex) => {
                if ( partIndex != pathParts.length - 1 ) {
                    if ( !currentNode.items.has(pathPart) ) {
                        currentNode.items.set(pathPart, {
                            name: pathPart,
                            type: Solution.ProjectTypeGuid.Directory,
                            guid: uuid.v4().toUpperCase(),
                            items: new Map()
                        });
                    }
                }
                else {
                    currentNode.items.set(project._projectName, {
                        name: project._projectName,
                        type: Solution.ProjectTypeGuid.Project,
                        project: project,
                        guid: project._guid
                    });
                }

                currentNode = currentNode.items.get(pathPart);
            });
        });
    }

    toSolutionContent() {
        const lines = [];
        
        lines.push('');
        lines.push(`Microsoft Visual Studio Solution File, Format Version ${this._formatVersion}`);
        lines.push('# Visual Studio 2013');
        lines.push(`VisualStudioVersion = ${this._vsVersion}`);
        lines.push(`MinimumVisualStudioVersion = ${this._minVsVersion}`);

        this._projects.forEach(project => {
            lines.push(project.toSolutionContent({
                typeGuid: Solution.ProjectTypeGuid.Project
            }));
        });

        this._projectDirectories.forEach(projectDirectory => {
            lines.push(projectDirectory.toSolutionContent());
        });

        lines.push('Global');
        this._globalSections.forEach(globalSection => {
            lines.push(globalSection.toSolutionContent());
        });
        lines.push('EndGlobal');

        lines.push('');

        return lines.join('\r\n');
    }
}

Solution.ProjectTypeGuid = {
    Project: '8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942',
    Directory: '2150E333-8FDC-42A3-9474-1A3956D46DE8'
};

class SolutionGlobalSection {
    constructor(options) {
        this._name = options.name;
        this._type = options.type;
        this._records = options.records;

        if ( !this._records ) {
            this._records = [];
        }
    }

    addRecord(record) {
        this._records.push(record);
    }

    toSolutionContent() {
        const lines = [];

        lines.push(`\tGlobalSection(${this._name}) = ${this._type}`);
        this._records.forEach(record => {
            lines.push(`\t\t${record.key} = ${record.value}`);
        });
        lines.push('\tEndGlobalSection');

        return lines.join('\r\n');
    }
}

class Project {
    static create(options) {
        return new Project(options);
    }

    constructor(options) {
        const guidKey = `projects.${options.rootNamespace}.guid`;

        this._cache = options.cache;
        this._defaultTargets = 'Build';
        this._toolsVersion = '14.0';
        this._filterToolVersion = '4.0';
        this._xmlns = 'http://schemas.microsoft.com/developer/msbuild/2003';

        if ( !this._cache.options[guidKey] ) {
            this._cache.options[guidKey] = uuid.v4().toUpperCase()
        }

        this._guid = this._cache.options[guidKey];
        this._keyword = options.keyword;
        this._rootNamespace = options.rootNamespace;
        this._projectName = options.rootNamespace;
        this._path = options.path;
        this._innerPath = options.innerPath;

        this._configurations = [];
        this._imports = [];
        this._importGroups = [];
        this._userMacros = [];

        this._files = {};
        this._internalDeps = [];

        this._projectPath = path.join(options.buildPath, options.path);
        this._projectDir = path.parse(this._projectPath).dir;
        this._kakeProjectOptions = options.projectOptions;

        logger.info(`Configure project: ${options.path}`);
        logger.info('Scanning header files');
        let includeFiles = this.findHeaderFiles(options.projectOptions.compiler.includePaths);
        this._includeFilters = util.fs.findFilesToFilters({
            name: 'Header files',
            basePath: options.projectOptions.basePath,
            projectDir: this._projectDir,
            files: includeFiles,
            exts: [ 'h', 'hh', 'hpp', 'hxx', 'hm', 'inl', 'inc', 'xsd' ]
        });

        logger.info('Scanning source files');
        this._sourceFilters = util.fs.findFilesToFilters({
            name: 'Source files',
            basePath: options.projectOptions.basePath,
            projectDir: this._projectDir,
            files: options.projectOptions.compiler.src,
            exts: [ 'cpp', 'c', 'cxx', 'def', 'odl', 'idl', 'bat', 'hpj', 'asm', 'asmx' ]
        });

        for ( let [filterName, filterInfo] of this._includeFilters ) {
            let files = filterInfo.files;
            let args = ['ClInclude'].concat(files);
            this.addFiles.apply(this, args);
        }

        for ( let [filterName, filterInfo] of this._sourceFilters ) {
            let files = filterInfo.files;
            let args = ['ClCompile'].concat(files);
            this.addFiles.apply(this, args);
        }

        if ( options.projectOptions.compiler.useCuda ) {
            logger.info('Scanning CUDA source files');

            this._cudaSourceFilters = util.fs.findFilesToFilters({
                name: 'CUDA Source files',
                basePath: options.projectOptions.basePath,
                projectDir: this._projectDir,
                files: options.projectOptions.compiler.nvccSrc,
                exts: [ 'cu' ]
            });

            for ( let [filterName, filterInfo] of this._cudaSourceFilters ) {
                let files = filterInfo.files;
                let args = ['CudaCompile'].concat(files);
                this.addFiles.apply(this, args);
            }
        }
    }

    addInternalDep(internalDep) {
        this._internalDeps.push(internalDep);
    }

    findHeaderFiles(includePaths) {
        let includeDirs = includePaths.filter(includePath => fs.statSync(includePath).isDirectory());
        includePaths = includePaths.filter(includePath => fs.statSync(includePath).isFile());

        includeDirs.forEach(includeDir => {
            let includeFiles = util.fs.findFiles({
                path: includeDir,
                exts: [ 'h', 'hpp' ]
            });

            includePaths = includePaths.concat(includeFiles);
        });

        return includePaths;
    }

    addConfiguration(configuration) {
        this._configurations.push(new ProjectConfiguration(configuration));

        return this;
    }

    addConfigurations(...configurations) {
        configurations.forEach(configuration => {
            this.addConfiguration(configuration);
        });

        return this;
    }

    addImport(importItem) {
        this._imports.push(new Import(importItem));

        return this;
    }

    addImports(...importItems) {
        importItems.forEach(importItem => {
            this._imports.push(new Import(importItem));
        });

        return this;
    }

    addImportGroup(importGroup) {
        this._importGroups.push(new ImportGroup(importGroup));

        return this;
    }

    addImportGroups(...importGroups) {
        importGroups.forEach(importGroup => {
            this._importGroups.push(new ImportGroup(importGroup));
        });

        return this;
    }

    addFile(fileType, fileName) {
        console.log(typeof(fileName));
        if ( !this._files[fileType] ) {
            this._files[fileType] = []; 
        }

        this._files[fileType].push(fileName);

        return this;
    }

    addFiles(fileType, ...files) {
        if ( !this._files[fileType] ) {
            this._files[fileType] = []; 
        }

        this._files[fileType] = this._files[fileType].concat(files);

        return this;
    }

    toFilterFile() {
        const filterFileNode = new XmlElement({
            name: 'Project'
        });

        const filterItems = [];
        const includeFilterFileItems = [];
        this._includeFilters.forEach(includeFilter => {
            const projectFilter = new ProjectFilter({
                include: includeFilter.include,
                uniqueIdentifier: uuid.v4().toUpperCase(),
                extensions: includeFilter.exts
            });

            filterItems.push(projectFilter);

            includeFilter.files.forEach(includeFilterFile => {
                const filterFile = new ProjectFilterFile({
                    path: includeFilterFile,
                    filter: includeFilter.include,
                    type: 'ClInclude'
                });

                includeFilterFileItems.push(filterFile);
            });
        });

        const sourceFilterFileItems = [];
        this._sourceFilters.forEach(sourceFilter => {
            const projectFilter = new ProjectFilter({
                include: sourceFilter.include,
                uniqueIdentifier: uuid.v4().toUpperCase(),
                extensions: sourceFilter.exts
            });

            filterItems.push(projectFilter);

            sourceFilter.files.forEach(sourceFilterFile => {
                const filterFile = new ProjectFilterFile({
                    path: sourceFilterFile,
                    filter: sourceFilter.include,
                    type: 'ClCompile'
                });

                sourceFilterFileItems.push(filterFile);
            });
        });

        const cudaFilterFileItems = [];
        if ( this._cudaSourceFilters ) {
            this._cudaSourceFilters.forEach(sourceFilter => {
                const projectFilter = new ProjectFilter({
                    include: sourceFilter.include,
                    uniqueIdentifier: uuid.v4().toUpperCase(),
                    extensions: sourceFilter.exts
                });

                filterItems.push(projectFilter);

                sourceFilter.files.forEach(sourceFilterFile => {
                    const filterFile = new ProjectFilterFile({
                        path: sourceFilterFile,
                        filter: sourceFilter.include,
                        type: 'CudaCompile'
                    });

                    cudaFilterFileItems.push(filterFile);
                });
            });
        }

        const filterItemGroup = new ItemGroup({
            items: filterItems
        });

        const includeItemGroup = new ItemGroup({
            items: includeFilterFileItems
        });

        const sourceItemGroup  = new ItemGroup({
            items: sourceFilterFileItems
        });

        filterFileNode
            .addAttribute('ToolsVersion', this._filterToolVersion)
            .addAttribute('xmlns', this._xmlns)
            .addChildren([
                    filterItemGroup.toXml(),
                    includeItemGroup.toXml(),
                    sourceItemGroup.toXml()
            ]);

        if ( this._cudaSourceFilters ) {
            const cudaItemGroup = new ItemGroup({
                items: cudaFilterFileItems
            });

            filterFileNode.addChild(cudaItemGroup.toXml());
        }

        return filterFileNode;
    }

    toProjectFile() {
        const projectNode = new XmlElement({
            name: 'Project'
        });

        const configurationIdentifiers = this._configurations.map(configuration => {
            return configuration.identifier;
        });

        const projectConfiguration = new ItemGroup({
            label: 'ProjectConfigurations',
            itemName: 'ProjectConfiguration',
            items: configurationIdentifiers
        });

        const globalProperties = new PropertyGroup({
            label: 'Globals',
            properties: {
                keyword: this._keyword,
                projectGuid: `{${this._guid}}`,
                rootNamespace: this._rootNamespace,
                projectName: this._projectName
            }
        });

        const cppDefaultProps = new Import({
            project: '$(VCTargetsPath)\\Microsoft.Cpp.Default.props'
        });

        const summaries = this._configurations.map(configuration => {
            const summary = new PropertyGroup({
                label: 'Configuration',
                condition: configuration.identifier.condition,
                properties: configuration.summary
            });

            return summary.toXml();
        });

        const cppProps = new Import({
            project: '$(VCTargetsPath)\\Microsoft.Cpp.props'
        });

        const extensionSettings = new ImportGroup({
            label: 'ExtensionSettings'
        });

        if ( this._kakeProjectOptions.compiler.useCuda ) {
            extensionSettings.importItems = [{
                project: '$(VCTargetsPath)\\BuildCustomizations\\CUDA 7.5.props'
            }];
        }

        const propertySheetsList = this._configurations.map(configuration => {
            const propertySheets = new ImportGroup(configuration.propertySheets);

            return propertySheets.toXml();
        });

        const userMacros = new PropertyGroup({
            label: 'UserMacros'
        });

        const basicLinkers = this._configurations.map(configuration => {
            const basicLinker = new PropertyGroup({
                condition: configuration.identifier.condition,
                properties: configuration.basicLinker
            });

            return basicLinker.toXml();
        });

        const itemDefinitionGroups = this._configurations.map(configuration => {
            const itemDefinitionGroupContent = {
                condition: configuration.identifier.condition,
                compiler: configuration.compiler,
                linker: configuration.otherLinker
            };

            if ( this._kakeProjectOptions.compiler.useCuda ) {
                itemDefinitionGroupContent.postBuildEvent = {
                    command: 
                        'echo copy "$(CudaToolkitBinDir)\\cudart*.dll" "$(OutDir)"\n copy "$(CudaToolkitBinDir)\\cudart*.dll" "$(OutDir)"'
                };

                if ( this._kakeProjectOptions.arch === 'x64' ) {
                    itemDefinitionGroupContent.cudaCompile = {
                        targetMachinePlatform: '64'
                    };
                }
            }

            const itemDefinitionGroup = 
                    new ItemDefinitionGroup(itemDefinitionGroupContent);

            return itemDefinitionGroup.toXml();
        });

        const cppTargetsProps = new Import({
            project: '$(VCTargetsPath)\\Microsoft.Cpp.targets'
        });

        const extensionTargets = new ImportGroup({
            label: 'ExtensionTargets'
        });

        if ( this._kakeProjectOptions.compiler.useCuda ) {
            extensionTargets.importItems = [{
                project: '$(VCTargetsPath)\\BuildCustomizations\\CUDA 7.5.targets'
            }];
        }

        const projectReferenceItems = this._internalDeps.map(internalDep => {
            return new ProjectReference({
                projectDir: this._projectDir,
                reference: internalDep
            });
        });

        const projectReferences = new ItemGroup({
            items: projectReferenceItems
        });

        const fileTypes = Object.keys(this._files);
        const fileItemGroups = fileTypes.map(fileType => {
            const files = this._files[fileType];

            const fileItems = files.map(file => {
                return new ProjectFile({
                    path: file,
                    type: fileType
                });
            });

            const fileItemGroup = new ItemGroup({
                items: fileItems
            });

            return fileItemGroup.toXml();
        });

        return projectNode
            .addAttribute('DefaultTargets', this._defaultTargets)
            .addAttribute('ToolsVersion', this._toolsVersion)
            .addAttribute('xmlns', this._xmlns)
            .addChildren([
                    projectConfiguration.toXml(),
                    globalProperties.toXml(),
                    cppDefaultProps.toXml(),
            ])
            .addChildren(summaries)
            .addChildren([
                    cppProps.toXml(),
                    extensionSettings.toXml()
            ])
            .addChildren(propertySheetsList)
            .addChild(userMacros.toXml())
            .addChildren(basicLinkers)
            .addChildren(itemDefinitionGroups)
            .addChildren(fileItemGroups)
            .addChildren([
                    projectReferences.toXml(),
                    cppTargetsProps.toXml(),
                    extensionTargets.toXml()
            ]);
    }

    toSolutionContent(options) {
        const lines = [];

        const key = `Project("{${options.typeGuid}}")`;
        const value = `"${this._rootNamespace}", "${this._path}", "{${this._guid}}"`
        lines.push(`${key} = ${value}`);
        lines.push('EndProject');

        return lines.join('\r\n');
    }
}

class SolutionDirectory {
    constructor(options) {
        this._guid = options.guid;
        this._name = options.name;
    }

    toSolutionContent(options) {
        const lines = [];

        const key = `Project("{${Solution.ProjectTypeGuid.Directory}}")`;
        const value = `"${this._name}", "${this._name}", "{${this._guid}}"`
        lines.push(`${key} = ${value}`);
        lines.push('EndProject');

        return lines.join('\r\n');
    }
}

class ProjectFilter {
    constructor(options) {
        this._include = options.include;
        this._uniqueIdentifier = options.uniqueIdentifier;
        if ( options.extensions ) {
            this._extensions = options.extensions.join(';');
        }
    }

    toXml() {
        const xmlElement = new XmlElement({
            name: 'Filter',
            attributes: [{
                name: 'Include',
                value: this._include
            }],
            children: [
                new XmlElement({
                    name: 'UniqueIdentifier',
                    text: `{${this._uniqueIdentifier}}`
                })
            ]
        });

        if ( this._extensions ) {
            const extensions = new XmlElement({
                name: 'Extensions',
                text: this._extensions
            });

            xmlElement.addChild(extensions);
        }

        return xmlElement;
    }
}

class PropertyGroup {
    constructor(options) {
        this._tag = 'PropertyGroup';
        if ( options.tag ) {
            this._tag = options.tag;
        }
        
        this._condition = options.condition;
        this._label = options.label;
        this._properties = options.properties;
    }

    toXml() {
        const xmlElement = new XmlElement({
            name: this._tag
        });

        if ( this._label ) {
            xmlElement.addAttribute('Label', this._label);
        }

        if ( this._condition ) {
            xmlElement.addAttribute('Condition', this._condition);
        }

        if ( this._properties ) {
            Object.keys(this._properties).forEach(key => {
                const xmlKey = key.slice(0, 1).toUpperCase() + key.slice(1);
                xmlElement.addChild(new XmlElement({
                    name: xmlKey,
                    text: this._properties[key]
                }));
            });
        }

        return xmlElement;
    }
}

class ItemDefinitionGroup {
    constructor(options) {
        this._condition = options.condition;
        this._label = options.label;
        this._compiler = options.compiler;
        this._linker = options.linker;
        this._postBuildEvent = options.postBuildEvent;
        this._cudaCompile = options.cudaCompile;
    }

    toXml() {
        const xmlElement = new XmlElement({
            name: 'ItemDefinitionGroup'
        });

        if ( this._label ) {
            xmlElement.addAttribute('Label', this._label);
        }

        if ( this._condition ) {
            xmlElement.addAttribute('Condition', this._condition);
        }

        const properties = {
            clCompile: this._compiler,
            link: this._linker
        };

        if ( this._postBuildEvent ) {
            properties.postBuildEvent = this._postBuildEvent;
        }

        if ( this._cudaCompile ) {
            properties.cudaCompile = this._cudaCompile;
        }

        Object.keys(properties).forEach(key => {
            const xmlKey = key.slice(0, 1).toUpperCase() + key.slice(1);
            const propertyGroup = new PropertyGroup({
                tag: xmlKey,
                properties: properties[key]
            });

            xmlElement.addChild(propertyGroup.toXml());
        });

        return xmlElement;
    }
}

class ItemGroup {
    constructor(options) {
        this._label = options.label;
        this._itemName = options.itemName;
        this._items = options.items;
    }

    toXml() {
        const itemsXml = this._items.map(item => {
            return item.toXml();
        });

        const xmlElement = new XmlElement({
            name: 'ItemGroup',
            children: itemsXml
        });

        if ( this._label ) {
            xmlElement.addAttribute('Label', this._label);
        }

        return xmlElement;
    }
}

class ImportGroup {
    constructor(options) {
        this._condition = options.condition;
        this._label = options.label;
        if ( options.importItems ) {
            this._importItems = options.importItems.map(importItem => {
                if ( importItem instanceof Import ) {
                    return importItem;
                }

                return new Import(importItem);
            });
        }
    }

    set importItems(importItems) {
        this._importItems = importItems.map(importItem => {
            if ( importItem instanceof Import ) {
                return importItem;
            }

            return new Import(importItem);
        });
    }

    toXml() {
        const xmlElement = new XmlElement({
            name: 'ImportGroup'
        });

        if ( this._label ) {
            xmlElement.addAttribute('Label', this._label);
        }

        if ( this._condition ) {
            xmlElement.addAttribute('Condition', this._condition);
        }

        if ( this._importItems ) {
            const importXmls = this._importItems.map(importItem => {
                return importItem.toXml()
            });

            xmlElement.children = importXmls;
        }

        return xmlElement;
    }
}

class Import {
    constructor(options) {
        this._project = options.project;
        this._condition = options.condition;
        this._label = options.label;
    }

    toXml() {
        const xmlElement = new XmlElement({
            name: 'Import',
        });

        if ( this._project ) {
            xmlElement.addAttribute('Project', this._project);
        }

        if ( this._label ) {
            xmlElement.addAttribute('Label', this._label);
        }

        if ( this._condition ) {
            xmlElement.addAttribute('Condition', this._condition);
        }

        return xmlElement;
    }
}

class ProjectConfiguration {
    constructor(options) {
        this._configuration = options.configuration;
        this._platform = options.platform;
        
        this._type = options.type;
        this._useDebugLibraries = options.useDebugLibraries;
        this._platformToolset = options.platformToolset;
        this._characterSet = options.characterSet;
        this._wholeProgramOptimization = options.wholeProgramOptimization;

        this._localAppDataPlatform = options.localAppDataPlatform;

        this._linker = {
            basic: {
                linkIncremental: options.linker.basic.linkIncremental,
                outDir: options.linker.basic.outDir,
                intDir: options.linker.basic.intDir
            },
            other: options.linker.other
        };

        this._compiler = options.compiler;

        this._identifier = new ConfigruationIdentifier({
            configuration: this._configuration,
            platform: this._platform
        });
    }

    get identifier() {
        return this._identifier;
    }

    get summary() {
        return {
            configurationType: this._type,
            useDebugLibraries: this._useDebugLibraries,
            platformToolset: this._platformToolset,
            characterSet: this._characterSet,
            wholeProgramOptimization: this._wholeProgramOptimization
        };
    }

    get otherLinker() {
        return this._linker.other;
    }

    get compiler() {
        return this._compiler;
    }

    get basicLinker() {
        return {
            linkIncremental: this._linker.basic.linkIncremental,
            outDir: this._linker.basic.outDir,
            intDir: this._linker.basic.intDir
        };
    }

    get propertySheets() {
        return {
            label: 'PropertySheets',
            condition: this.identifier.condition,
            importItems: [{
                project: this._localAppDataPlatform,
                condition: `exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')`,
                label: 'LocalAppDataPlatform'
            }]
        };
    }
}

class ConfigruationIdentifier {
    constructor(options) {
        this._configuration = options.configuration;
        this._platform = options.platform;
    }

    get condition() {
        return `'$(Configuration)|$(Platform)'=='${this._configuration}|${this._platform}'`;
    }

    toString() {
        return `${this._configuration}|${this._platform}`;
    }

    toXml() {
        return new XmlElement({
            name: 'ProjectConfiguration',
            attributes: [{
                name: 'Include',
                value: this.toString()
            }],
            children: [
                new XmlElement({
                    name: 'Configuration',
                    text: this._configuration
                }),
                new XmlElement({
                    name: 'Platform',
                    text: this._platform
                })
            ]
        });
    }
}

class ProjectFile {
    constructor(options) {
        this._path = options.path;
        this._type = options.type;
    }

    get path() {
        return this._path;
    }

    get type() {
        return this._type;
    }

    toXml() {
        return new XmlElement({
            name: this._type,
            attributes: [{
                name: 'Include',
                value: this._path
            }]
        });
    }
}

class ProjectFilterFile extends ProjectFile {
    constructor(options) {
        super(options);
        this._filter = options.filter;
    }

    get filter() {
        return this._filter;
    }

    toXml() {
        const xmlElement = super.toXml();

        return xmlElement
            .addChild(new XmlElement({
                name: 'Filter',
                text: this._filter
            }));
    }
}

class ProjectReference {
    constructor(options) {
        this._projectDir = options.projectDir;
        this._reference = options.reference;
    }

    get reference() {
        return this._reference;
    }

    toXml() {
        return new XmlElement({
            name: 'ProjectReference',
            attributes: [{
                name: 'Include',
                value: util.fs.relativePath(this._reference._projectPath, this._projectDir)
            }],
            children: [
                new XmlElement({
                    name: 'Project',
                    text: `{${this._reference._guid}}`
                })
            ]
        });
    }
}

ConfigruationIdentifier.conditionName = '$(Configuration)|$(Platform)';

module.exports = {
    Solution,
    Project
};
