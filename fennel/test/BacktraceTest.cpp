/*
// $Id$
// Fennel is a library of data storage and processing components.
// Copyright (C) 2005-2005 The Eigenbase Project
// Copyright (C) 2005-2005 Disruptive Tech
// Copyright (C) 2005-2005 LucidEra, Inc.
// Portions Copyright (C) 1999-2005 John V. Sichi
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version approved by The Eigenbase Project.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "fennel/common/CommonPreamble.h"
#include "fennel/common/Backtrace.h"
#include "fennel/test/TestBase.h"
#include "fennel/common/TraceSource.h"

#include <boost/test/test_tools.hpp>
#include <boost/scoped_array.hpp>

using namespace fennel;

class BacktraceTest : virtual public TestBase, public TraceSource
{
    void testDeliberateBacktrace();
    void testBacktraceOnAbort();
public:
    explicit BacktraceTest() 
        : TraceSource(shared_from_this(),"BacktraceTest")
    {
        FENNEL_UNIT_TEST_CASE(BacktraceTest, testDeliberateBacktrace);
        // FENNEL_UNIT_TEST_CASE(BacktraceTest, testBacktraceOnAbort);
    }
    
    virtual ~BacktraceTest()
    {
    }
};

void BacktraceTest::testDeliberateBacktrace()
{
    Backtrace bt;
    FENNEL_TRACE(TRACE_INFO, "this is a backtrace" << std::endl << bt);
}

void BacktraceTest::testBacktraceOnAbort()
{
    FENNEL_TRACE(TRACE_INFO, "asserting to force a backtrace");
    assert(false);
}

FENNEL_UNIT_TEST_SUITE(BacktraceTest);

