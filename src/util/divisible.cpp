/*********************                                                        */
/*! \file divisible.cpp
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/

#include "util/divisible.h"
#include "util/exception.h"

using namespace std;

namespace CVC4 {

Divisible::Divisible(const Integer& n) : k(n) {
  CheckArgument(n > 0, n, "Divisible predicate must be constructed over positive N");
}

}/* CVC4 namespace */
