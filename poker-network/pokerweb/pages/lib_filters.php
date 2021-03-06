<?php
//
// Copyright (C) 2005, 2006 Mekensleep <licensing@mekensleep.com>
//                          24 rue vieille du temple, 75004 Paris
//
// This software's license gives you freedom; you can copy, convey,
// propagate, redistribute and/or modify this program under the terms of
// the GNU Affero General Public License (AGPL) as published by the Free
// Software Foundation (FSF), either version 3 of the License, or (at your
// option) any later version of the AGPL published by the FSF.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero
// General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program in a file in the toplevel directory called
// "AGPLv3".  If not, see <http://www.gnu.org/licenses/>.
//
// Authors:
//  Morgan Manach <akshell@free.fr>
//
	/***
		String cleaning from unwanted char.
		could probably be improve with ereg_replace.
	***/

	$trans_array = array();
	for ($i = 0; $i < 32; $i++)
		$trans_array[chr($i)] = ' ';
	unset($trans_array["\t"]);
	unset($trans_array["\r"]);
	unset($trans_array["\n"]);

	function strclean ($str) {
		global $trans_array;
		return strtr($str, $trans_array);
	}

	/***
		Escaping POST, COOKIE & GET vars
	***/
	function _cookie_numeric($nom, $defaut = null) {
		return isset($_COOKIE[$nom]) && is_numeric($_COOKIE[$nom])?$_COOKIE[$nom]:$defaut;
	}

	function _cookie_string($nom, $defaut = '') {
		if (isset($_COOKIE[$nom])) {
			$item = $_COOKIE[$nom];
			if (get_magic_quotes_gpc())
				$item = stripslashes($item);
			return strclean($item);
		} else
			return $defaut;
	}

	function _get_numeric($nom, $defaut = null) {
		return isset($_GET[$nom]) && is_numeric($_GET[$nom])?$_GET[$nom]:$defaut;
	}

	function _get_string($nom, $defaut = '') {
		if (isset($_GET[$nom])) {
			$item = $_GET[$nom];
			if (get_magic_quotes_gpc())
				$item = stripslashes($item);
			return strclean($item);
		} else
			return $defaut;
	}

	function _get_array_string($nom) {
		$array = isset($_GET[$nom]) && is_array($_GET[$nom])?$_GET[$nom]:null;
		foreach ($array as $clef => $item) {
			if (get_magic_quotes_gpc())
				$item = stripslashes($item);
			$array[$clef] = strclean($item);
		}
		return $array;
	}

	function _get_array_numeric($nom) {
		$array = isset($_GET[$nom]) && is_array($_GET[$nom])?$_GET[$nom]:null;
		foreach ($array as $clef => $item)
			if (!is_numeric($item))
				unset($array[$clef]);
		return $array;
	}

	function _post_numeric($nom, $defaut = null) {
		return isset($_POST[$nom]) && is_numeric($_POST[$nom])?$_POST[$nom]:$defaut;
	}

	function _post_string($nom, $defaut = '') {
		if (isset($_POST[$nom])) {
			$item = $_POST[$nom];
			if (get_magic_quotes_gpc())
				$item = stripslashes($item);
			return strclean($item);
		} else
			return $defaut;
	}

	function _post_array_string($nom) {
		$array = isset($_POST[$nom]) && is_array($_POST[$nom])?$_POST[$nom]:null;
		if (isset($array))
			foreach ($array as $clef => $item) {
				if (get_magic_quotes_gpc())
					$item = stripslashes($item);
				$array[$clef] = strclean($item);
			}
		return $array;
	}

	function _post_array_numeric($nom) {
		$array = isset($_POST[$nom]) && is_array($_POST[$nom])?$_POST[$nom]:null;
		if (isset($array))
			foreach ($array as $clef => $item)
				if (!is_numeric($item))
					unset($array[$clef]);
		return $array;
	}

function _me() {
  if(isset($_SERVER["HTTPS"]) && $_SERVER['SERVER_PORT'] == '443') {
    $url = "https://";
  } else {
    $url = "http://";
  }

  $url .= $_SERVER['SERVER_NAME'];

  if(!(isset($_SERVER["HTTPS"]) && $_SERVER['SERVER_PORT'] == '443') &&
     !(!isset($_SERVER["HTTPS"]) && $_SERVER['SERVER_PORT'] == '80')) {
    $url .= ":" . $_SERVER['SERVER_PORT'];
  }

  $url .= $_SERVER["SCRIPT_NAME"];

  return $url;
}

?>
