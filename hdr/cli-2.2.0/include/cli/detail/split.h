/*******************************************************************************
 * CLI - A simple command line interface.
 * Copyright (C) 2016-2021 Daniele Pallastrelli
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************/

#ifndef CLI_DETAIL_SPLIT_H_
#define CLI_DETAIL_SPLIT_H_

#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <cassert>

namespace cli
{
namespace detail
{

class Text
{
public:
    explicit Text(std::string _input) : input(std::move(_input))
    {
    }
    void SplitInto(std::vector<std::string>& strs)
    {
        Reset();
        for (char c: input)
            Eval(c);
        RemoveEmptyEntries();
        splitResult.swap(strs); // puts the result back in strs
    }
private:
    void Reset()
    {
        state = State::space;
        prev_state = State::space;
        sentence_type = SentenceType::double_quote;
        splitResult.clear();
    }

    void Eval(char c)
    {
        switch(state)
        {
            case State::space:
                EvalSpace(c);
                break;
            case State::word:
                EvalWord(c);
                break;
            case State::sentence:
                EvalSentence(c);
                break;
            case State::escape:
                EvalEscape(c);
                break;
        }
    }

    void EvalSpace(char c)
    {
        if (c == ' ' || c == '\t' || c == '\n')
        {
            // do nothing
        }
        else if (c == '"' || c == '\'')
        {
            NewSentence(c);
        }
        else if (c == '\\')
        {
            // This is the case where the first character of a word is escaped.
            // Should come back into the word state after this.
            prev_state = State::word;
            state = State::escape;
            splitResult.emplace_back("");
        }
        else if (c == '!')
        {
            splitResult.emplace_back(1, c);
        }
        else
        {
            state = State::word;
            splitResult.emplace_back(1, c);
        }
    }

    void EvalWord(char c)
    {
        if (c == ' ' || c == '\t' || c == '\n')
        {
            state = State::space;
        }
        else if (c == '!')
        {
            splitResult.emplace_back(1, c);
            state = State::space;
        }
        else if (c == '"' || c == '\'')
        {
            NewSentence(c);
        }
        else if (c == '\\')
        {
            prev_state = state;
            state = State::escape;
        }      
        else
        {
            assert(!splitResult.empty());
            splitResult.back() += c;
        }
    }

    void EvalSentence(char c)
    {
        if (c == '"' || c == '\'')
        {
            auto new_type = c == '"' ? SentenceType::double_quote : SentenceType::quote;
            if (new_type == sentence_type)
                state = State::space;
            else
            {
                assert(!splitResult.empty());
                splitResult.back() += c;
            }
        }
        else if (c == '\\')
        {
            prev_state = state;
            state = State::escape;
        }
        else
        {
            assert(!splitResult.empty());
            splitResult.back() += c;
        }
    }

    void EvalEscape(char c)
    {
        assert(!splitResult.empty());
        if (c != '"' && c != '\'' && c != '\\')
            splitResult.back() += "\\";
        splitResult.back() += c;
        state = prev_state;
    }

    void NewSentence(char c)
    {
        state = State::sentence;
        sentence_type = ( c == '"' ? SentenceType::double_quote : SentenceType::quote);
        splitResult.emplace_back("");
    }

    void RemoveEmptyEntries()
    {
        // remove null entries from the vector:
        splitResult.erase(
            std::remove_if(
                splitResult.begin(),
                splitResult.end(),
                [](const std::string& s){ return s.empty(); }
            ),
            splitResult.end()
        );
    }

    enum class State { space, word, sentence, escape };
    enum class SentenceType { quote, double_quote };
    State state = State::space;
    State prev_state = State::space;
    SentenceType sentence_type = SentenceType::double_quote;
    const std::string input;
    std::vector<std::string> splitResult;
};

// Split the string input into a vector of strings.
// The original string is split where there are spaces.
// Quotes and double quotes can be used to indicate a substring that should not be splitted
// (even if it contains spaces)

//          split(strs, "");                => empty vector
//          split(strs, " ");               => empty vector
//          split(strs, "  ");              => empty vector
//          split(strs, "\t");              => empty vector
//          split(strs, "  \t \t     ");    => empty vector

//          split(strs, "1234567890");      => <"1234567890">
//          split(strs, "  foo ");          => <"foo">
//          split(strs, "  foo \t \t bar \t");  => <"foo","bar">

//          split(strs, "\"\"");            => empty vector
//          split(strs, "\"foo bar\"");     => <"foo bar">
//          split(strs, "    \t\t \"foo \tbar\"     \t");   => <"foo \tbar">
//          split(strs, " first   \t\t \"foo \tbar\"     \t last"); => <"first","foo \tbar","last">
//          split(strs, "first\"foo \tbar\"");  => <"first","foo \tbar">
//          split(strs, "first \"'second' 'thirdh'\""); => <"first","'second' 'thirdh'">

//          split(strs, "''");      => empty vector
//          split(strs, "'foo bar'");   => <"foo bar">
//          split(strs, "    \t\t 'foo \tbar'     \t"); => <"foo \tbar">
//          split(strs, " first   \t\t 'foo \tbar'     \t last"); => <"first","foo \tbar","last">
//          split(strs, "first'foo \tbar'"); => <"first","foo \tbar">
//          split(strs, "first '\"second\" \"thirdh\"'"); => <"first","\"second\" \"thirdh\"">

//          split(strs, R"("foo\"bar")"); // "foo\"bar" => <"foo"bar">
//          split(strs, R"('foo\'bar')"); // 'foo\'bar' => <"foo'bar">
//          split(strs, R"("foo\bar")"); // "foo\bar" => <"foo\bar">
//          split(strs, R"("foo\\"bar")"); // "foo\\"bar" => <"foo\"bar">

inline void split(std::vector<std::string>& strs, const std::string& input)
{
    Text sentence(input);
    sentence.SplitInto(strs);
}

} // namespace detail
} // namespace cli

#endif // CLI_DETAIL_SPLIT_H_
