﻿/// <reference path="API.ts" />

/// <reference path="library/xml/XML.ts" />
/// <reference path="collections/ArrayCollection.ts" />
/// <reference path="protocol/entity/Entity.ts" />
/// <reference path="templates/distributed/DistributedSystemArrayMediator.ts" />

try
{
	// IF THE CONDITION BE IS_NODE(), THEN CANNOT BE USED IN BROWSERIFY
	module.exports = samchon;
} 
catch (exception) { }