/*********************                                                        */
/*! \file tptp_printer.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief The pretty-printer interface for the TPTP output language
 **
 ** The pretty-printer interface for the TPTP output language.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__PRINTER__TPTP_PRINTER_H
#define __CVC4__PRINTER__TPTP_PRINTER_H

#include <iostream>

#include "printer/printer.h"

namespace CVC4 {
namespace printer {
namespace tptp {

class TptpPrinter : public CVC4::Printer {
  void toStream(std::ostream& out, const Model& m, const Command* c) const throw();
public:
  using CVC4::Printer::toStream;
  void toStream(std::ostream& out, TNode n, int toDepth, bool types, size_t dag) const throw();
  void toStream(std::ostream& out, const Command* c, int toDepth, bool types, size_t dag) const throw();
  void toStream(std::ostream& out, const CommandStatus* s) const throw();
  void toStream(std::ostream& out, const SExpr& sexpr) const throw();
  void toStream(std::ostream& out, const Model& m) const throw();
  void toStream(std::ostream& out, const Result& r) const throw();
};/* class TptpPrinter */

}/* CVC4::printer::tptp namespace */
}/* CVC4::printer namespace */
}/* CVC4 namespace */

#endif /* __CVC4__PRINTER__TPTP_PRINTER_H */
