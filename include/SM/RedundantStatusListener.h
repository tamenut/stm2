#pragma once

#include "SMCommon.h"

class RedundantStatusListener
{
public:
	RedundantStatusListener(void){};
	~RedundantStatusListener(void){};

    virtual void changed_redundant_status(REDUNDANT_STATUS redundant_status) = 0;
};
