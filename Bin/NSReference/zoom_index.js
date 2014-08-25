dictwords = ["aliases 0 16 132",
	"top 0 10 2",
	"next 0 10 1",
	"points 0 10 1 5 12 16",
	"rect 0 10 1 5 33 12",
	"aliases.htm 0 6 64",
	"teleport 1 41 48",
	"the 1 18 48 4 10 4",
	"robot 1 18 48 2 18 8 4 10 4",
	"point 1 47 56 2 27 32 5 26 16",
	"note 1 10 32 3 10 8",
	"alias 1 18 40 2 100 252 3 10 8",
	"can 1 18 40 2 10 32 3 10 8 4 10 8",
	"used 1 18 40 3 10 8",
	"define 1 18 40 3 10 8",
	"examples 1 18 20 2 47 60 3 10 4 5 33 52",
	"declare 1 18 20 2 124 60 3 10 4",
	"auto 1 26 28 2 18 48 5 60 60",
	"reversed 1 10 16 2 18 48 5 99 60",
	"origin 1 18 16 2 10 32",
	"300 1 10 16 5 18 4",
	"450 1 10 16",
	"90deg 1 10 16",
	"send 1 10 16 4 10 8",
	"speed 1 33 14",
	"value 1 10 8 5 33 36",
	"direction 1 18 12",
	"forward 1 18 12",
	"backward 1 18 12",
	"notes 1 10 8",
	"default 1 18 12",
	"100 1 10 8",
	"1000 1 10 4 2 10 32 5 18 20",
	"1500 1 18 4 2 10 32 5 10 8",
	"center 1 18 4 2 10 32",
	"1800 1 18 4",
	"2300 1 10 4",
	"movements 1 14 192",
	"reference 1 8 128 2 8 128 3 8 128 4 8 128 5 8 128",
	"movements.htm 1 6 64",
	"are 2 10 64 4 10 4",
	"custom 2 10 64",
	"variable 2 10 32",
	"which 2 10 32 4 10 8",
	"use 2 10 32",
	"shortcuts 2 10 32",
	"name 2 47 60",
	"rectangle 2 27 48 5 26 12",
	"500 2 18 16 5 18 4",
	"startzone 2 10 16",
	"800 2 10 16",
	"1300 2 10 16",
	"200 2 18 16 5 18 12",
	"centerzone 2 10 16",
	"sensor 2 34 16 3 79 60",
	"color 2 10 16 3 10 4",
	"leftsensor 2 10 8",
	"microswitch 2 10 8 3 18 6",
	"middleswitch 2 10 8",
	"parameter 2 34 8",
	"maxspeed 2 10 8",
	"action 2 34 12",
	"actuator 2 10 8",
	"closeclaws 2 10 4",
	"ax-12 2 27 4",
	"ax12 2 10 4",
	"leftax12 2 10 4",
	"alias.htm 2 6 64",
	"activation 3 12 32",
	"activate 3 18 48",
	"enable 3 40 54",
	"type 3 47 56",
	"deactivate 3 18 24",
	"disable 3 33 30",
	"all 3 47 26",
	"sensors 3 29 216",
	"sharp 3 10 4",
	"middlesharp 3 18 6",
	"colorsensor 3 10 2",
	"sensors.htm 3 6 64",
	"neobot 4 20 16",
	"script 4 20 16",
	"interpreted 4 10 8",
	"language 4 10 8",
	"generate 4 10 8",
	"actions 4 10 8",
	"that 4 10 8",
	"basic 4 12 4 5 14 192",
	"syntax 4 12 4",
	"statements 4 10 4",
	"separated 4 10 2",
	"carriage 4 10 2",
	"return 4 10 2",
	"commented 4 10 2",
	"lines 4 10 2",
	"starts 4 10 2",
	"introduction 4 14 192",
	"syntax.htm 4 6 64",
	"angles 5 12 32",
	"fixed 5 26 56",
	"angle 5 47 48",
	"deg 5 26 32",
	"rad 5 26 32",
	"45deg 5 10 32",
	"1.5rad 5 10 32",
	"72deg 5 10 16",
	"250 5 10 8",
	"rectangles 5 12 8",
	"width 5 26 12",
	"height 5 26 12",
	"revsed 5 10 4",
	"time 5 20 4",
	"500ms 5 10 2",
	"types 5 14 192",
	"basic_types.htm 5 6 64"];
skipwords = ["and,or,the,it,is,an,on,we,us,to,of,"];
var STR_FORM_SEARCHFOR = "Search for:";
var STR_FORM_SUBMIT_BUTTON = "Submit";
var STR_FORM_RESULTS_PER_PAGE = "Results per page:";
var STR_FORM_MATCH = "Match:";
var STR_FORM_ANY_SEARCH_WORDS = "any search words";
var STR_FORM_ALL_SEARCH_WORDS = "all search words";
var STR_NO_QUERY = "No search query entered.";
var STR_RESULTS_FOR = "Search results for:";
var STR_NO_RESULTS = "No results";
var STR_RESULT = "result";
var STR_RESULTS = "results";
var STR_PHRASE_CONTAINS_COMMON_WORDS = "Your search query contained too many common words to return the entire set of results available. Please try again with a more specific query for better results.";
var STR_SKIPPED_FOLLOWING_WORDS = "The following word(s) are in the skip word list and have been omitted from your search:";
var STR_SKIPPED_PHRASE = "Note that you can not search for exact phrases beginning with a skipped word";
var STR_SUMMARY_NO_RESULTS_FOUND = "No results found.";
var STR_SUMMARY_FOUND_CONTAINING_ALL_TERMS = "found containing all search terms.";
var STR_SUMMARY_FOUND_CONTAINING_SOME_TERMS = "found containing some search terms.";
var STR_SUMMARY_FOUND = "found.";
var STR_PAGES_OF_RESULTS = "pages of results.";
var STR_POSSIBLY_GET_MORE_RESULTS = "You can possibly get more results searching for";
var STR_ANY_OF_TERMS = "any of the terms";
var STR_DIDYOUMEAN = "Did you mean:";
var STR_SORTEDBY_RELEVANCE = "Sorted by relevance";
var STR_SORTBY_RELEVANCE = "Sort by relevance";
var STR_SORTBY_DATE = "Sort by date";
var STR_SORTEDBY_DATE = "Sorted by date";
var STR_RESULT_TERMS_MATCHED = "Terms matched:";
var STR_RESULT_SCORE = "Score:";
var STR_RESULT_URL = "URL:";
var STR_RESULT_PAGES = "Result Pages:";
var STR_RESULT_PAGES_PREVIOUS = "Previous";
var STR_RESULT_PAGES_NEXT = "Next";
var STR_FORM_CATEGORY = "Category:";
var STR_FORM_CATEGORY_ALL = "All";
var STR_RESULTS_IN_ALL_CATEGORIES = "in all categories";
var STR_RESULTS_IN_CATEGORY = "in category";
var STR_POWEREDBY = "Search powered by";
var STR_MORETHAN = "More than";
var STR_ALL_CATS = "all categories";
var STR_CAT_SUMMARY = "Refine your search by category:";
var STR_OR = "or";
var STR_RECOMMENDED = "Recommended links";
var STR_SEARCH_TOOK = "Search took";
var STR_SECONDS = "seconds";
var STR_MAX_RESULTS = "You have requested more results than served per query. Please try again with a more precise query.";
