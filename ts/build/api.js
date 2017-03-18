const fs = require("fs");
const os = require("os");
const process = require('child_process');

/**
 * Directory to release the API documents.
 * 
 * Specify this constant value where you want.
 */
const RELEASE_DIR = "D:/Homepage/samchon.github.io/framework/api/ts";

if (fs.existsSync(RELEASE_DIR))
	if (os.platform().substr(0, 3) == "win")
		process.execSync("rd " + RELEASE_DIR.split("/").join("\\") + " /S /Q");
	else
		process.execSync("rm -rf " + RELEASE_DIR);

// var command = 
// 	"typedoc --tsconfig ../src/samchon/tsconfig.json " + 
// 	"--target ES5 --mode file --out " + RELEASE_DIR + " " +
// 	"--includeDeclarations --excludeExternals --externalPattern \"**/+(node|websocket)*\"";

var command = 
	"typedoc ../node_modules/tstl/lib/tstl.d.ts ../lib/samchon.d.ts " + 
	"--target ES5 --mode file --out " + RELEASE_DIR + " " +
	"--includeDeclarations --excludeExternals --externalPattern \"**/+(node|websocket)*\"";

process.execSync(command);