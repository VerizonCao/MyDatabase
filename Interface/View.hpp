//  Created by rick gessner on 4/17/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.
//
//  interface for view, like tableView, databaseView.......

#ifndef View_h
#define View_h

#include <iostream>

namespace ECE141 {

  //completely generic view, which you will subclass to show information
  class View {
  public:
    virtual         ~View() {}
    virtual bool    show(std::ostream &aStream)=0;
  };

}

#endif /* View_h */
