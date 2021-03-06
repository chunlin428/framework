#pragma once

#include <string>
#include <iostream>
#include <sstream>

#include <samchon/WeakString.hpp>
#include <samchon/IndexPair.hpp>
#include <samchon/library/Math.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief Utility class for string 
	 * 
	 * @details 
	 * <p> StringUtil is an utility class providing lots of static methods for std::string. </p>
	 * 
	 * <p> There are two methods to strength std::string to have addictional uility methods like trim and split. 
	 * The way of first is to make std::string class inheriting from std::string. 
	 * The second is to make StringUtil class having static methods. </p>
	 *
	 * <p> But those methods have problems. std::string class violates standard and StringUtil class violates 
	 * principle of Object-Oriented Design. For the present, I've made the StringUtil class, but if you 
	 * have a good opinion about the issue, please write your opinion on my github. </p>
	 *
	 * @image html cpp/subset/library_string.png
	 * @image latex cpp/subset/library_string.png
	 *
	 * @see library::WeakString
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class StringUtil
	{
	public:
		/* ----------------------------------------------------------------------
			SUBSTITUTE
		---------------------------------------------------------------------- */
		/**
		 * @brief Substitutes &quot;{n}&quot; tokens within the specified string with the respective arguments passed in.
		 *
		 * @param format The string to make substitutions in.\n
		 *				 This string can contain special tokens of the form {n}, where n is a zero based index,
		 *				 that will be replaced with the additional parameters found at that index if specified
		 * @param val Target value to substitute the minimum {n} tokens
		 * @param args Additional parameters that can be substituted in the str parameter at each {n} location,
		 *			   where n is an integer (zero based) index value into the varadics of values specified.
		 * @return New string with all of the {n} tokens replaced with the respective arguments specified.
		 */
		template <typename T, typename ... _Args>
		static auto substitute(const std::string &format,
			const T& val, const _Args& ... args) -> std::string
		{
			std::string &res = _substitute(format, val);

			return StringUtil::substitute(res, args...);
		};
		template <typename T> static auto substitute(const std::string &format, const T& val) -> std::string
		{
			return _substitute(format, val);
		};

		/**
		 * @brief Substitutes &quot;{n}&quot; tokens within the specified sql-string with the respective arguments passed in.\n
		 * @warning substituteSQL creates the dynamic sql-statement.\n
		 *			Not recommended when the dynamic sql-statement is not only for procedure.
		 *
		 * @param format The string to make substitutions in.\n
		 *				 This string can contain special tokens of the form {n}, where n is a zero based index,
		 *				 that will be replaced with the additional parameters found at that index if specified
		 * @param val Target value to substitute the minimum {n} tokens
		 * @param args Additional parameters that can be substituted in the str parameter at each {n} location,
		 *			   where n is an integer (zero based) index value into the varadics of values specified.
		 * @return New sql-string with all of the {n} tokens replaced with the respective arguments specified.
		 */
		template <typename T, typename ... _Args >
		static auto substituteSQL(const std::string &format,
			const T& value, const _Args& ... args) -> std::string
		{
			std::string &res = _substituteSQL(format, value);
			return StringUtil::substituteSQL(res, args...);
		};
		template <typename T> static auto substituteSQL(const std::string &format, const T& value) -> std::string
		{
			return _substituteSQL(format, value);
		};

	protected:
		template <typename T> static auto _substitute(const std::string &format, const T& value) -> std::string
		{
			std::vector<std::string> &parenthesisArray = betweens(format, { (char)'{' }, { (char)'}' });
			std::vector<long> vec;

			for (auto it = parenthesisArray.begin(); it != parenthesisArray.end(); it++)
				if (isNumeric(*it) == true)
					vec.push_back(stoi(*it));

			size_t index = (size_t)Math::minimum(vec).getValue();

			//replaceAll
			std::string &to = toString(value);
			return replaceAll(format, "{" + std::to_string(index) + "}", to);
		};

		template <typename T> static auto _substituteSQL(const std::string &format, const T& value) -> std::string
		{
			std::vector<std::string> &parenthesisArray = betweens(format, "{", "}");
			std::vector<long> vec;

			for (auto it = parenthesisArray.begin(); it != parenthesisArray.end(); it++)
				if (isNumeric(*it) == true)
					vec.push_back(stoi(*it));

			size_t index = (size_t)Math::minimum(vec).getValue();

			//replaceAll
			std::string &to = toSQL(value);
			return replaceAll(format, "{" + std::to_string(index) + "}", to);
		};

		/* ----------------------------------------------------------------------
			SUBSTITUTE -> TO_STRING
		---------------------------------------------------------------------- */
		template <typename T>
		static auto toString(const T &val) -> std::string
		{
			return std::to_string(val);
		};
		template<> static auto toString(const std::string &str) -> std::string
		{
			return str;
		};
		template<> static auto toString(const WeakString &str) -> std::string
		{
			return str.str();
		};
		static auto toString(const char *ptr) -> std::string
		{
			return ptr;
		};

		template <typename T>
		static auto toSQL(const T &val) -> std::string
		{
			if (val == INT_MIN)
				return "NULL";

			return std::to_string(val);
		};
		template<> static auto toSQL(const bool &flag) -> std::string
		{
			return std::to_string(flag);
		};
		template<> static auto toSQL(const char &val) -> std::string
		{
			return toSQL(std::string({ val }));
		};
		template<> static auto toSQL(const std::string &str) -> std::string
		{
			return toSQL(WeakString(str));
		};
		template<> static auto toSQL(const WeakString &wstr) -> std::string
		{
			if (wstr.empty() == true)
				return "NULL";
			else
			{
				if (wstr.find("'") != std::string::npos)
					return "'" + wstr.replaceAll("'", "''") + "'";
				else
					return "'" + wstr.str() + "'";
			}
		};
		static auto toSQL(const char *ptr) -> std::string
		{
			return toSQL(std::string(ptr));
		};

	public:
		/* ----------------------------------------------------------------------
			NUMBER-FORMAT
				IN MONETARY UNIT, ADD DELIMETER ','
				COLOR-FORMAT

			POSITIVE NUMBER IS RED,
				NEGATIVE NUMBER IS BLUE
				ZERO IS BLACK
		---------------------------------------------------------------------- */
		/**
		 * @brief Returns wheter the std::string represents Number or not\n
		 *
		 * @param str Target std::string to check
		 * @return Whether the std::string can be converted to Number or not
		 */
		static auto isNumeric(const std::string &str) -> bool
		{
			try
			{
				stoi(str);
				//stod( replaceAll(str, ",", "") );
			}
			catch (const std::exception &)
			{
				return false;
			}
			catch (...)
			{
				return false;
			}

			return true;
		};

		/**
		 * @brief Number std::string to Number having ',' symbols
		 *
		 * @param str Target std::string you want to convert to Number
		 * @return Number from std::string
		 */
		static auto toNumber(const std::string &str) -> double
		{
			std::string &numStr = replaceAll(str, ",", "");

			return stod(numStr);
		};

		/**
		 * @brief
		 *
		 * @details
		 * Returns a string converted from the number rounded off from specified precision with &quot;,&quot; symbols
		 * &nbsp;&nbsp;&nbsp;&nbsp; ex) numberFormat(17151.339, 2) => 17,151.34
		 *
		 * @param val A number wants to convert to string
		 * @param precision Target precision of roundoff
		 * @return A string representing the number with roundoff and &quot;,&quot; symbols
		 */
		static auto numberFormat(double val, int precision = 2) -> std::string
		{
			std::string str;

			// FIRST, DO ROUND-OFF
			val = round(val * pow(10, precision));
			val = val / pow(10, precision);

			// SEPERATE NUMBERS
			bool is_negative = (val < 0);
			unsigned long long natural = (unsigned long long)abs(val);
			double fraction = abs(val) - (unsigned long long)abs(val);

			// NATURAL NUMBER
			if (natural == 0)
				str = "0";
			else
			{
				// NOT ZERO
				size_t cipher_count = (size_t)log10(natural) + 1;

				for (size_t i = 0; i <= cipher_count; i++)
				{
					size_t cipher = natural % (size_t)pow(10, i + 1);
					cipher = (size_t)(cipher / pow(10, i));

					if (i == cipher_count && cipher == 0)
						continue;

					// IS MULTIPLIER OF 3
					if (i > 0 && i % 3 == 0)
						str = "," + str;

					// PUSH FRONT TO THE STRING
					str = std::to_string(cipher) + str;
				}
			}

			// NEGATIVE SIGN
			if (is_negative == true)
				str = "-" + str;

			// ADD FRACTION
			if (precision > 0 && fraction != 0)
			{
				fraction = (double)(unsigned long long)round(fraction * pow(10, precision));
				size_t zeros = precision - (size_t)log10(fraction) - 1;

				str += "." + std::string(zeros, '0') + std::to_string((unsigned long long)fraction);
			}
			return str;
		};

		/**
		 * @brief
		 * Returns a percentage string converted from the number rounded off from specified precision with &quot;,&quot; symbols\n
		 * &nbsp;&nbsp;&nbsp;&nbsp; ex) percentFormat(11.3391, 1) => 1,133.9%
		 *
		 * @warning Do not multiply by 100 to the value representing percent
		 * @param val A number wants to convert to percentage string
		 * @param precision Target precision of roundoff
		 */
		static auto percentFormat(double val, int precision = 2) -> std::string
		{
			if (val == INT_MIN)
				return "";
			return numberFormat(val * 100, precision) + "%";
		};

		/**
		 * @brief
		 * Returns a string converted from the number rounded off from specified precision with &quot;,&quot; symbols and color tag\n
		 * &nbsp;&nbsp;&nbsp;&nbsp; ex) numberFormat(17151.339, 2) => <font color='red'>17,151.34</font>
		 *
		 * @details
		 * Which color would be chosen
		 *	\li Number is positive, color is RED
		 *	\li Number is zero (0), color is BLACK
		 *	\li Number is negative, color is BLUE
		 *
		 * @param val A number wants to convert to colored string
		 * @param precision Target precision of roundoff
		 * @return A colored string representing the number with roundoff and &quot;,&quot; symbols
		 */
		static auto colorNumberFormat(double value, int precision = 2, double delimiter = 0.0) -> std::string
		{
			std::string color;

			if (value > delimiter)			color = "red";
			else if (value == delimiter)	color = "black";
			else							color = "blue";

			return substitute
			(
				"<font color='{1}'>{2}</font>",
				color,
				numberFormat(value, precision)
			);
		};

		/**
		 * @brief Returns a percentage string converted from the number rounded off from specified precision with &quot;,&quot; symbols\n
		 * &nbsp;&nbsp;&nbsp;&nbsp; ex) percentFormat(11.3391, 1) => 1,133.9%
		 *
		 * @warning Do not multiply by 100 to the value representing percent
		 * @param val A number wants to convert to percentage string
		 * @param precision Target precision of roundoff
		 */
		static auto colorPercentFormat(double value, int precision = 2, double delimiter = 0.0) -> std::string
		{
			std::string color;

			if (value > delimiter)			color = "red";
			else if (value == delimiter)	color = "black";
			else							color = "blue";

			return substitute
			(
				"<font color='{1}'>{2}</font>",
				color,
				percentFormat(value, precision)
			);
		};

		/* ----------------------------------------------------------------------
			TRIM -> WITH LTRIM & RTRIM
				IT'S RIGHT, THE TRIM OF ORACLE
		---------------------------------------------------------------------- */
		/**
		 * @brief Removes all designated characters from the beginning and end of the specified string
		 *
		 * @param str The string should be trimmed
		 * @param delims Designated character(s)
		 * @return Updated string where designated characters was removed from the beginning and end
		 */
		static auto trim(const std::string &val, const std::vector<std::string> &delims) -> std::string
		{
			return WeakString(val).trim(delims).str();
		};

		/**
		 * @brief Removes all designated characters from the beginning of the specified string
		 *
		 * @param str The string should be trimmed
		 * @param delims Designated character(s)
		 * @return Updated string where designated characters was removed from the beginning
		 */
		static auto ltrim(const std::string &val, const std::vector<std::string> &delims) -> std::string
		{
			return WeakString(val).ltrim(delims).str();
		};

		/**
		 * @brief Removes all designated characters from the end of the specified string
		 *
		 * @param str The string should be trimmed
		 * @param delims Designated character(s)
		 * @return Updated string where designated characters was removed from the end
		 */
		static auto rtrim(const std::string &val, const std::vector<std::string> &delims) -> std::string
		{
			return WeakString(val).rtrim(delims).str();
		};

		static auto trim(const std::string &str) -> std::string
		{
			return WeakString(str).trim().str();
		};
		static auto ltrim(const std::string &str) -> std::string
		{
			return WeakString(str).ltrim().str();
		};
		static auto rtrim(const std::string &str) -> std::string
		{
			return WeakString(str).rtrim().str();
		};

		static auto trim(const std::string &str, const std::string &delim) -> std::string
		{
			return WeakString(str).trim(delim).str();
		};
		static auto ltrim(const std::string &str, const std::string &delim) -> std::string
		{
			return WeakString(str).ltrim(delim).str();
		};
		static auto rtrim(const std::string &str, const std::string &delim) -> std::string
		{
			return WeakString(str).rtrim(delim).str();
		};

		/* ----------------------------------------------------------------------
			EXTRACTORS
		---------------------------------------------------------------------- */
		/**
		 * @brief Finds first occurence in string
		 *
		 * @details
		 * Finds first occurence position of each delim in the string after startIndex
		 * and returns the minimum position of them\n
		 * \n
		 * If startIndex is not specified, then starts from 0.\n
		 * If failed to find any substring, returns -1 (std::string::npos)
		 *
		 * @param str Target string to find
		 * @param delims The substrings of target(str) which to find
		 * @param startIndex Specified starting index of find. Default is 0
		 * @return pair\<size_t := position, string := matched substring\>
		 */
		static auto finds(const std::string &str,
			const std::vector<std::string> &delims, size_t startIndex = 0) -> IndexPair<std::string>
		{
			IndexPair<WeakString> &iPair = WeakString(str).finds(delims, startIndex);

			return { iPair.get_index(), iPair.getValue().str() };
		};

		/**
		 * @brief Finds last occurence in string
		 *
		 * @details
		 * Finds last occurence position of each delim in the string before endIndex
		 * and returns the maximum position of them\n
		 * \n
		 * If index is not specified, then starts str.size() - 1\n
		 * If failed to find any substring, returns -1 (std::string::npos)
		 *
		 * @param str Target string to find
		 * @param delims The substrings of target(str) which to find
		 * @param endIndex Specified starting index of find. Default is str.size() - 1
		 * @return pair\<size_t := position, string := matched substring\>
		*/
		static auto rfinds(const std::string &str,
			const std::vector<std::string> &delims, size_t endIndex = SIZE_MAX) -> IndexPair<std::string>
		{
			IndexPair<WeakString> &iPair = WeakString(str).rfinds(delims, endIndex);

			return { iPair.get_index(), iPair.getValue().str() };
		};

		/**
		 * @brief Generates a substring
		 *
		 * @details
		 * Extracts a string consisting of the character specified by startIndex and all characters up to endIndex - 1
		 * If endIndex is not specified, string::size() will be used instead.\n
		 * If endIndex is greater than startIndex, then those will be swapped
		 *
		 * @param str Target string to be applied substring
		 * @param startIndex Index of the first character.\n
		 *					 If startIndex is greater than endIndex, those will be swapped
		 * @param endIndex Index of the last character - 1.\n
						   If not specified, then string::size() will be used instead
		 * @return Extracted string by specified index(es)
		 */
		static auto substring(const std::string &str,
			size_t startIndex, size_t endIndex = SIZE_MAX) -> std::string
		{
			return WeakString(str).substring(startIndex, endIndex).str();
		};

		/**
		 * @brief Generate a substring.
		 *
		 * @details
		 * <p> Extracts a substring consisting of the characters from specified start to end
		 * It's same with str.substring( ? = (str.find(start) + start.size()), str.find(end, ?) ) </p>
		 *
		 * <p> ex) between("ABCD[EFGH]IJK", "[", "]") => "EFGH" </p>
		 *
		 * \li If start is not specified, extracts from begin of the string to end.
		 * \li If end is not specified, extracts from start to end of the string.
		 * \li If start and end are all omitted, returns str, itself.
		 *
		 * @param str Target string to be applied between
		 * @param start A string for separating substring at the front
		 * @param end A string for separating substring at the end
		 *
		 * @return substring by specified terms
		 */
		static auto between(const std::string &str,
			const std::string &start = "", const std::string &end = "") -> std::string
		{
			return WeakString(str).between(start, end).str();
		};

		//TAB
		/**
		 * @brief Adds tab(\t) character to first position of each line
		 *
		 * @param str Target str to add tabs
		 * @param n The size of tab to be added for each line
		 * @return A string added multiple tabs
		 */
		static auto addTab(const std::string &str, size_t n = 1) -> std::string
		{
			std::vector<std::string> &lines = split(str, "\n");

			std::string val;
			std::string tab;
			size_t i;

			val.reserve(val.size() + lines.size());
			tab.reserve(n);

			for (i = 0; i < n; i++)
				tab += "\t";

			for (i = 0; i < lines.size(); i++)
				val.append(tab + lines[i] + ((i == lines.size() - 1) ? "" : "\n"));

			return val;
		};

		//MULTIPLE STRINGS
		/**
		 * @brief Generates substrings
		 * @details Splits a string into an array of substrings by dividing the specified delimiter
		 *
		 * @param str Target string to split
		 * @param delim The pattern that specifies where to split this string
		 * @return An array of substrings
		 */
		static auto split(const std::string &str, const std::string &delim) -> std::vector<std::string>
		{
			std::vector<WeakString> &arr = WeakString(str).split(delim);

			std::vector<std::string> resArray(arr.size());
			for (size_t i = 0; i < arr.size(); i++)
				resArray[i] = move(arr[i].str());

			return resArray;
		};

		/**
		 * @brief Generates substrings
		 *
		 * @details
		 * <p> Splits a string into an array of substrings dividing by specified delimeters of start and end.
		 * It's the array of substrings adjusted the between. </p>
		 *
		 *	\li If startStr is omitted, it's same with the split by endStr not having last item
		 *	\li If endStr is omitted, it's same with the split by startStr not having first item
		 *	\li If startStr and endStar are all omitted, returns {str}
		 *
		 * @param str Target string to split by between
		 * @param start A string for separating substring at the front.
		 *				If omitted, it's same with split(end) not having last item
		 * @param end A string for separating substring at the end.
		 *			  If omitted, it's same with split(start) not having first item
		 * @return An array of substrings
		 */
		static auto betweens
			(
				const std::string &str,
				const std::string &start = "", const std::string &end = ""
			) -> std::vector<std::string>
		{
			std::vector<WeakString> &arr = WeakString(str).betweens(start, end);

			std::vector<std::string> resArray(arr.size());
			for (size_t i = 0; i < arr.size(); i++)
				resArray[i] = move(arr[i].str());

			return resArray;
		};

		/* ----------------------------------------------------------------------
			REPLACERS
		---------------------------------------------------------------------- */
		//ALPHABET-CONVERSION
		/**
		 * @brief Returns a string that all uppercase characters are converted to lowercase.
		 *
		 * @param str Target string to convert uppercase to lowercase
		 * @return A string converted to lowercase
		 */
		static auto toLowerCase(const std::string &str) -> std::string
		{
			return WeakString(str).toLowerCase();
		};

		/**
		 * Returns a string all lowercase characters are converted to uppercase\n
		 *
		 * @param str Target string to convert lowercase to uppercase
		 * @return A string converted to uppercase
		 */
		static auto yoUpperCase(const std::string &str) -> std::string
		{
			return WeakString(str).yoUpperCase();
		};

		/**
		 * @brief Returns a string specified word is replaced
		 *
		 * @param str Target string to replace
		 * @param before Specific word you want to be replaced
		 * @param after Specific word you want to replace
		 * @return A string specified word is replaced
		 */
		static auto replaceAll
			(
				const std::string &str, 
				const std::string &before, const std::string &after
			) -> std::string
		{
			return WeakString(str).replaceAll(before, after);
		};

		/**
		 * @brief Returns a string specified words are replaced
		 *
		 * @param str Target string to replace
		 * @param pairs A specific word's pairs you want to replace and to be replaced
		 * @return A string specified words are replaced
		 */
		static auto replaceAll(const std::string &str,
			const std::vector<std::pair<std::string, std::string>> &pairs) -> std::string
		{
			return WeakString(str).replaceAll(pairs);
		};

		/**
		 * @brief Replace all HTML spaces to a literal space.
		 *
		 * @param str Target string to replace.
		 */
		static auto removeHTMLSpaces(const std::string &str) -> std::string
		{
			std::vector<std::pair<std::string, std::string>> pairs =
			{
				{ "&nbsp;", " " },
				{ "\t", " " },
				{ "  ", " " }
			};
			return replaceAll(str, pairs);
		};
	};
};
};