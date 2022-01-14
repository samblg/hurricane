'use strict';

const fs = require('fs');

class XmlGenerator {
    constructor(options) {
        this._version = options.version;
        this._encoding = options.encoding;

        if ( options.indent ) {
            this._indent = options.indent;
        }
        else {
            this._indent = 2;
        }
    }

    write(filePath, root) {
        const fd = fs.openSync(filePath, 'w');

        this.writeHeader(fd);
        if ( root ) {
            root.write(fd, {
                indent: {
                    depth: 0,
                    width: this._indent
                }
            });
        }

        fs.closeSync(fd);
    }

    writeHeader(fd) {
        const headerText = `<?xml version="${this._version}" encoding="${this._encoding}"?>\n`;

        fs.writeSync(fd, headerText);
    }
}

class XmlElement {
    constructor(options) {
        this._name = options.name;

        if ( options.attributes ) {
            this._attributes = options.attributes;
        }
        else {
            this._attributes = [];
        }
        this._attributesMap = new Map();
        this._attributes.forEach(attribute => {
            this._attributesMap.set(attribute.name, attribute.value);
        });

        if ( options.text !== undefined && options.text !== null ) {
            this._children = [
                new XmlTextElement({
                    text: options.text
                })
            ];
        }
        else {
            if ( options.children ) {
                this._children = options.children;
            }
            else {
                this._children = [];
            }
        }
    }

    get attributes() {
        return this._attributes;
    }

    get attributesMap() {
        return this._attributesMap;
    }

    get children() {
        return this._children;
    }

    set children(children) {
        this._children = children;
    }

    addAttribute(name, value) {
        this._attributes.push({
            name: name,
            value: value
        });
        this._attributesMap.set(name, value);

        return this;
    }

    removeAttribute(name) {
        this._attributes = this._attributes.filter(attribute => {
            return attribute.name != name
        });
        this._attributesMap.delete(name);

        return this;
    }

    addChild(child) {
        this._children.push(child);

        return this;
    }

    addChildren(children) {
        this._children = this._children.concat(children);

        return this;
    }
    
    write(fd, options) {
        const indent = ' '.repeat(options.indent.depth * options.indent.width);
        const attributeText = this.makeAttributesText();

        if ( this.children.length == 1 && this.children[0] instanceof XmlTextElement ) {
            const openTag = `${indent}<${this._name}${attributeText}>`;
            const closeTag = `</${this._name}>\n`;

            fs.writeSync(fd, openTag);
            this._children[0].write(fd);
            fs.writeSync(fd, closeTag);

            return;
        }

        const openTag = `${indent}<${this._name}${attributeText}>\n`;
        const closeTag = `${indent}</${this._name}>\n`;

        fs.writeSync(fd, openTag);

        this._children.forEach(child => {
            child.write(fd, {
                indent: {
                    depth: options.indent.depth + 1,
                    width: options.indent.width
                }
            });
        });

        fs.writeSync(fd, closeTag);
    }

    makeAttributesText() {
        if ( !this._attributes.length ) {
            return '';
        }

        const attributePairs = this._attributes.map(attribute => {
            return `${attribute.name}="${attribute.value}"`;
        });

        return ' ' + attributePairs.join(' ');
    }
}

class XmlTextElement {
    constructor(options) {
        this._text = options.text;
    }

    write(fd) {
        fs.writeSync(fd, this._text);
    }
}

module.exports = {
    XmlGenerator,
    XmlElement
};
