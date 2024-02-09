#ifndef REQUESTTARGET_HPP
#define REQUESTTARGET_HPP

#include <string>

namespace Hafserv
{

class RequestTarget
{
public:
	RequestTarget(std::string requestTarget);
	~RequestTarget();

private:
	std::string targetURI;
	std::string queryString;
};

} // namespace Hafserv

#endif
