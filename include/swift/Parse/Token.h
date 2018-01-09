//===--- Token.h - Token interface ------------------------------*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2015 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
//
//  This file defines the Token interface.
//
//===----------------------------------------------------------------------===//

#ifndef SWIFT_TOKEN_H
#define SWIFT_TOKEN_H

#include "llvm/Support/SMLoc.h"
#include "llvm/ADT/StringRef.h"

namespace swift {

namespace tok {
  enum TokenKind {
    unknown = 0,
    eof,
    identifier,
    
#define KEYWORD(X) kw_ ## X,
    // Types.
    KEYWORD(int)
//    KEYWORD(uint)
//    KEYWORD(int8)
//    KEYWORD(uint8)
//    KEYWORD(int16)
//    KEYWORD(uint16)
//    KEYWORD(int64)
//    KEYWORD(uint64)
//    KEYWORD(float)
    
    KEYWORD(var)

//    KEYWORD(foreach)
#undef KEYWORD
    
#define PUNCTUATOR(X, Y) X,
    PUNCTUATOR(comma,       ",")
    PUNCTUATOR(colon,       ":")
    PUNCTUATOR(semi,        ";")
    PUNCTUATOR(equal,       "=")
    PUNCTUATOR(plus,        "+")
    PUNCTUATOR(minus,       "-")
    PUNCTUATOR(star,        "*")
    PUNCTUATOR(slash,       "/")
    PUNCTUATOR(l_paren,     "(")
    PUNCTUATOR(r_paren,     ")")
    
#undef PUNCTUATOR
    
    NUM_TOKENS
  };
} // end namespace tok.

/// Token - This structure provides full information about a lexed token.
/// It is not intended to be space efficient, it is intended to return as much
/// information as possible about each returned token.  This is expected to be
/// compressed into a smaller form if memory footprint is important.
///
class Token {
  /// Kind - The actual flavor of token this is.
  ///
  tok::TokenKind Kind;
  
  /// Text - The actual string covered by the token in the source buffer.
  llvm::StringRef Text;
  
public:
  
  tok::TokenKind getKind() const { return Kind; }
  void setKind(tok::TokenKind K) { Kind = K; }
  
  /// is/isNot - Predicates to check if this token is a specific kind, as in
  /// "if (Tok.is(tok::l_brace)) {...}".
  bool is(tok::TokenKind K) const { return Kind == K; }
  bool isNot(tok::TokenKind K) const { return Kind != K; }
  
  /// getLocation - Return a source location identifier for the specified
  /// offset in the current file.
  llvm::SMLoc getLocation() const {
    return llvm::SMLoc::getFromPointer(Text.begin());
  }

  llvm::StringRef getText() const { return Text; }
  void setText(llvm::StringRef T) { Text = T; }
  
  unsigned getLength() const { return Text.size(); }
  
  /// setToken - Set the token to the specified kind and source range.
  ///
  void setToken(tok::TokenKind K, llvm::StringRef T) {
    Kind = K;
    Text = T;
  }
};
  
} // end namespace swift


namespace llvm {
  template <typename T> struct isPodLike;
  template <>
  struct isPodLike<swift::Token> { static const bool value = true; };
}  // end namespace llvm

#endif
