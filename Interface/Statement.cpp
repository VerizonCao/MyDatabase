//  Created by rick gessner on 3/20/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.


#include "Statement.hpp"
#include "../Tokenizer.hpp"

namespace ECE141 {
  
  Statement::Statement(Keywords aStatementType) : stmtType(aStatementType)  {
  }
  
  Statement::Statement(const Statement &aCopy) : stmtType(aCopy.stmtType) {
  }
  
  Statement::~Statement() {
  }

  //basic methods
  StatusResult Statement::parse(Tokenizer &aTokenizer) {
    return StatusResult{};
  }
  
  StatusResult Statement::run(std::ostream &aStream) const {
    return StatusResult();
  }
  
}
