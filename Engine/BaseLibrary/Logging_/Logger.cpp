#include "Logger.hpp"
#include "LogNode.hpp"

#include <cassert>

#include <iostream>
using std::cout;
using std::endl;

namespace exc {

Logger::Logger() {
	myNode = std::make_unique<LogNode>();
	outputFile = std::make_unique<std::ofstream>();
}

Logger::~Logger() {

}

bool Logger::OpenFile(const std::string& path) {
	std::ofstream newStream(path, std::ios::out | std::ios::trunc);
	if (!newStream.is_open()) {
		myNode->SetOutputStream(nullptr);
		return false;
	}
	else {
		myNode->SetOutputStream(nullptr);
		outputFile->close();
		*outputFile = std::move(newStream);
		myNode->SetOutputStream(outputFile.get());
		return true;
	}
}

void Logger::OpenStream(std::ostream* stream) {
	myNode->SetOutputStream(stream);
	outputFile->close();
}

void Logger::CloseStream() {
	myNode->SetOutputStream(nullptr);
	outputFile->close();
}

LogStream Logger::CreateLogStream(const std::string& name) {
	std::shared_ptr<LogPipe> pipe = myNode->CreatePipe(name);
	return LoggerInterface::Construct(pipe);
}

void Logger::Flush() {
	myNode->Flush();
}




} // namespace exc