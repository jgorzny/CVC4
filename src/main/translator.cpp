/*********************                                                        */
/*! \file translator.cpp
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2014  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Main driver for CVC4 translator executable
 **
 ** Main driver for CVC4 translator executable.
 **/

#include <iostream>
#include <fstream>
#include <getopt.h>
#include <cstring>
#include "util/language.h"
#include "expr/command.h"
#include "expr/expr.h"
#include "parser/parser_builder.h"
#include "parser/parser.h"

using namespace std;
using namespace CVC4;
using namespace CVC4::language;
using namespace CVC4::parser;

enum {
  INPUT_LANG = 'L',
  OUTPUT_LANG = 'O',
  OUTPUT_FILE = 'o',
  HELP = 'h'
};/* enum */

const struct option longopts[] = {
  { "output-lang", required_argument, NULL, OUTPUT_LANG },
  { "output-language", required_argument, NULL, OUTPUT_LANG },
  { "lang", required_argument, NULL, INPUT_LANG },
  { "language", required_argument, NULL, INPUT_LANG },
  { "out", required_argument, NULL, OUTPUT_FILE },
  { "help", no_argument, NULL, HELP },
  { NULL, no_argument, NULL, 0 },
};/* longopts */

static void showHelp() {
  cerr << "cvc4-translator translation tool" << endl
       << "  --output-language | -O  set output language (default smt2)" << endl
       << "  --input-language | -L   set input language (default auto)" << endl
       << "  --out | -o              set output file (- for stdout)" << endl
       << "  --help | -h             this help" << endl
       << "Options and input filenames can be intermixed, and order is important." << endl
       << "For instance, \"-O smt2 x -O cvc4 y\" reads file x in smt2 format and"
       << "file y in cvc4 format and writes all output to stdout." << endl
       << "Some canonicalization may occur." << endl
       << "Comments and formatting are not preserved." << endl;
}

static void readFile(const char* filename, InputLanguage fromLang, OutputLanguage toLang, ostream* out) {
  if(fromLang == input::LANG_AUTO) {
    unsigned len = strlen(filename);
    if(len >= 5 && !strcmp(".smt2", filename + len - 5)) {
      fromLang = language::input::LANG_SMTLIB_V2;
    } else if(len >= 4 && !strcmp(".smt", filename + len - 4)) {
      fromLang = language::input::LANG_SMTLIB_V1;
    } else if(len >= 5 && !strcmp(".smt1", filename + len - 5)) {
      fromLang = language::input::LANG_SMTLIB_V1;
    } else if((len >= 2 && !strcmp(".p", filename + len - 2)) ||
              (len >= 5 && !strcmp(".tptp", filename + len - 5))) {
      fromLang = language::input::LANG_TPTP;
    } else if(( len >= 4 && !strcmp(".cvc", filename + len - 4) ) ||
              ( len >= 5 && !strcmp(".cvc4", filename + len - 5) )) {
      fromLang = language::input::LANG_CVC4;
    } else {
      throw Exception("cannot determine input language to use for `" + string(filename) + "'");
    }
  }

  if(toLang == output::LANG_AUTO) {
    toLang = toOutputLanguage(fromLang);
  }

  *out << Expr::setlanguage(toLang);

  Options opts;
  opts.set(options::inputLanguage, fromLang);
  ExprManager exprMgr(opts);
  ParserBuilder parserBuilder(&exprMgr, filename, opts);
  if(!strcmp(filename, "-")) {
    parserBuilder.withFilename("<stdin>");
    parserBuilder.withLineBufferedStreamInput(cin);
  }
  Parser *parser = parserBuilder.build();
  while(Command* cmd = parser->nextCommand()) {
    *out << cmd << endl;
    if(dynamic_cast<QuitCommand*>(cmd) != NULL) {
      delete cmd;
      break;
    }
    delete cmd;
  }
  *out << flush;
  delete parser;
}

/**
 * Translate from an input language to an output language.
 */
int main(int argc, char* argv[]) {
  ostream* out = &cout;
  InputLanguage fromLang = input::LANG_AUTO;
  OutputLanguage toLang = output::LANG_SMTLIB_V2;
  size_t files = 0;

  try {
    int c;
    while((c = getopt_long(argc, argv, "-L:O:o:h", longopts, NULL)) != -1) {
      switch(c) {
      case 1:
        ++files;
        readFile(optarg, (!strcmp(optarg, "-") && fromLang == input::LANG_AUTO) ? input::LANG_CVC4 : fromLang, toLang, out);
        break;
      case INPUT_LANG:
        if(!strcmp(optarg, "help")) {
          Options::printLanguageHelp(cerr);
          exit(1);
        }
        fromLang = toInputLanguage(optarg);
        break;
      case OUTPUT_LANG:
        if(!strcmp(optarg, "help")) {
          Options::printLanguageHelp(cerr);
          exit(1);
        }
        toLang = toOutputLanguage(optarg);
        break;
      case OUTPUT_FILE:
        out->flush();
        if(out != &cout) {
          ((ofstream*)out)->close();
          delete out;
        }
        if(strcmp(optarg, "-")) {
          out = new ofstream(optarg);
        } else {
          out = &cout;
        }
        break;
      case 'h':
        showHelp();
        exit(0);
      case '?':
        showHelp();
        exit(1);
      default:
        cerr << "internal error.  translator failed ("
             << char(c) << "," << c << ")." << endl;
        exit(1);
      }
    }

    if(files == 0) {
      readFile("-", fromLang == input::LANG_AUTO ? input::LANG_CVC4 : fromLang, toLang, out);
      exit(0);
    }
  } catch(Exception& e) {
    cerr << e << endl;
    exit(1);
  }

  return 0;
}
