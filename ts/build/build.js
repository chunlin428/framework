const fs = require("fs");
const process = require('child_process');

compile();
attach_header();
remove_dynamics();
minify();

function compile()
{
	process.execSync("tsc -p ../src/samchon/tsconfig.json");
	process.execSync("tsc -p ../src/samchon/tsconfig.json  --removeComments --declaration false");
}

function attach_header()
{
	const TITLE_FILE = "../src/samchon/typings/samchon/samchon.d.ts";
	const HEADER_FILE = "../lib/samchon.d.ts";

	var text = fs.readFileSync(TITLE_FILE, "utf8");
	text += fs.readFileSync(HEADER_FILE, "utf8");
	text = text.replace("/// <reference types=\"tstl\" />", "");

	fs.writeFileSync(HEADER_FILE, text, "utf8");
}

function remove_dynamics()
{
	const JS_FILE = "../lib/samchon.js";

	var text = fs.readFileSync(JS_FILE, "utf8");
	if (text.indexOf('["') == -1)
		return;

	var dynamics = text.split('["');
	var used_keys = {};

	for (var i = 1; i < dynamics.length; i++)
	{
		if (dynamics[i].indexOf('"]') == -1)
			continue;
		
		var value = dynamics[i].substr(0, dynamics[i].indexOf('"]'));
		var org = '["' + value + '"]';
		var repl = '.' + value;

		if (value.indexOf(",") != -1)
			continue;
		else if (used_keys[value])
			continue;
		else
			used_keys[value] = true;
		
		text = text.split(org).join(repl);
	}
	fs.writeFileSync(JS_FILE, text, "utf8");
}

function minify()
{
	process.execSync("minify ../lib/samchon.js");
}
