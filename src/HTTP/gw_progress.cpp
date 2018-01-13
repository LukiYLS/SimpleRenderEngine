/* -*-c++-*- */
/* gw_http - Dynamic map generation toolkit for OpenSceneGraph
 * Copyright 2008-2013 Pelican Mapping
 * http://gw_http.org
 *
 * gw_http is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A fieldICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include <iostream>

#include "gw_progress.h"

using namespace std;
using namespace gw_http;

progress_callback::progress_callback() :
_canceled      ( false ),
_needsRetry    ( false )
{
    //NOP
}

bool progress_callback::report_progress(double             current,
                                      double             total,
                                      unsigned           stage,
                                      unsigned           numStages,
                                      const std::string& msg )
{
    return false;
}

/******************************************************************************/
console_progress_callback::console_progress_callback() :
progress_callback()
{
    //NOP
}

bool
console_progress_callback::report_progress(double current, double total, 
                                        unsigned stage, unsigned numStages,
                                        const std::string& msg)
{
    if (total > 0)
    {
        double percentComplete = (current / total) * 100.0;
        cout 
            << "Stage " << (stage+1) << "/" << numStages 
            << "; completed " << percentComplete << "% " << current << " of " << total 
            << endl;
    }
    else
    {
        cout << msg << endl;
    }
    return false;
}
