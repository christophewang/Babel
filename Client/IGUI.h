#ifndef __IGUI_H__
#define __IGUI_H__

#include <QtGui>
#include "Client.h"

class               IGUI: public Client
{
public:
    virtual         ~IGUI() {}
public:
    virtual void    manageWin() = 0;
};

#endif // IGUI_H
