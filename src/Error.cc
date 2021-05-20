/*
 * Copyright 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <memory>
#include "vendor/backward.hpp"

#include "sdf/Error.hh"

using namespace sdf;

class sdf::Error::Implementation
{
  /// \brief The error code value.
  public: ErrorCode code = ErrorCode::NONE;

  /// \brief Description of the error.
  public: std::string message = "";

  /// \brief Xml path where the error was raised.
  public: std::optional<std::string> xmlPath = std::nullopt;

  /// \brief File path where the error was raised.
  public: std::optional<std::string> filePath = std::nullopt;

  /// \brief Line number in the file path where the error was raised.
  public: std::optional<int> lineNumber = std::nullopt;

  public: std::shared_ptr<backward::StackTrace> st;
};

/////////////////////////////////////////////////
Error::Error()
  : dataPtr(ignition::utils::MakeImpl<Implementation>())
{}

/////////////////////////////////////////////////
Error::Error(const ErrorCode _code, const std::string &_message)
  : dataPtr(ignition::utils::MakeImpl<Implementation>())
{
  this->dataPtr->code = _code;
  this->dataPtr->message = _message;
  this->dataPtr->st = std::make_shared<backward::StackTrace>();
  this->dataPtr->st->load_here(10);
  this->dataPtr->st->skip_n_firsts(3);
}
std::string Error::Trace() const
{
  if (this->dataPtr->st)
  {
    std::stringstream ss;
    backward::Printer p;
    p.color_mode = backward::ColorMode::always;
    p.print(*this->dataPtr->st, ss);
    return ss.str();
  }
  return "";
}

/////////////////////////////////////////////////
Error::Error(const ErrorCode _code, const std::string &_message,
             const std::string &_filePath)
  : Error(_code, _message)
{
  this->dataPtr->filePath = _filePath;
}

/////////////////////////////////////////////////
Error::Error(const ErrorCode _code, const std::string &_message,
             const std::string &_filePath, int _lineNumber)
  : Error(_code, _message, _filePath)
{
  this->dataPtr->lineNumber = _lineNumber;
}

/////////////////////////////////////////////////
ErrorCode Error::Code() const
{
  return this->dataPtr->code;
}

/////////////////////////////////////////////////
std::string Error::Message() const
{
  return this->dataPtr->message;
}

/////////////////////////////////////////////////
std::optional<std::string> Error::FilePath() const
{
  return this->dataPtr->filePath;
}

/////////////////////////////////////////////////
std::optional<int> Error::LineNumber() const
{
  return this->dataPtr->lineNumber;
}

/////////////////////////////////////////////////
std::optional<std::string> Error::XmlPath() const
{
  return this->dataPtr->xmlPath;
}

/////////////////////////////////////////////////
void Error::SetXmlPath(const std::string &_xmlPath)
{
  this->dataPtr->xmlPath = _xmlPath;
}

/////////////////////////////////////////////////
// cppcheck-suppress unusedFunction
Error::operator bool() const
{
  return this->dataPtr->code != ErrorCode::NONE;
}

/////////////////////////////////////////////////
bool Error::operator==(const bool _value) const
{
  return ((this->dataPtr->code != ErrorCode::NONE) && _value) ||
         ((this->dataPtr->code == ErrorCode::NONE) && !_value);
}
