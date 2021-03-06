#!/usr/bin/env python
# -*- py-indent-offset: 4; coding: utf-8; mode: python -*-
# more information about the above line at http://www.python.org/dev/peps/pep-0263/
#
# Copyright (C) 2009 Bradley M. Kuhn <bkuhn@ebb.org>
#
# This software's license gives you freedom; you can copy, convey,
# propagate, redistribute and/or modify this program under the terms of
# the GNU Affero General Public License (AGPL) as published by the Free
# Software Foundation (FSF), either version 3 of the License, or (at your
# option) any later version of the AGPL published by the FSF.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero
# General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program in a file in the toplevel directory called
# "AGPLv3".  If not, see <http://www.gnu.org/licenses/>.
#

import unittest, sys
from os import path

TESTS_PATH = path.dirname(path.realpath(__file__))
sys.path.insert(0, path.join(TESTS_PATH, ".."))

from pokernetwork import nullfilter

# ------------------------------------------------------------    
class NullFilterTestCase(unittest.TestCase):
    # -------------------------------------------------------------------------
    def setUp(self):
        pass
    # -------------------------------------------------------------------------
    def tearDown(self):
        pass
    # -------------------------------------------------------------------------
    def test00_restfilter_none(self):
        """test00_restfilter_none"""

        self.failUnless(nullfilter.rest_filter(None, None, None),
                        "nullfilter.rest_filter always returns True")
    # -------------------------------------------------------------------------
    def test01_restfilter_strings(self):
        """test01_restfilter_strings"""

        self.failUnless(nullfilter.rest_filter("SITE", "REQUEST", "PACKET"),
                        "nullfilter.rest_filter always returns True")
# ------------------------------------------------------------
def GetTestSuite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(NullFilterTestCase))

    # Comment out above and use line below this when you wish to run just
    # one test by itself (changing prefix as needed).
#    suite.addTest(unittest.makeSuite(PokerGameHistoryTestCase, prefix = "test2"))
    return suite
# -----------------------------------------------------------------------------
def Run(verbose = 1):
    return unittest.TextTestRunner(verbosity=verbose).run(GetTestSuite())
# -----------------------------------------------------------------------------
if __name__ == '__main__':
    if Run().wasSuccessful():
        sys.exit(0)
    else:
        sys.exit(1)
