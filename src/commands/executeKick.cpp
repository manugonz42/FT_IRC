
static bool	isValid(const ParsedCommand& cmd)
{
	int size = cmd.params.size();
	if (size != 4)
		return false;
	return true;
}

bool	Server::executeKick(Client* client, const ParsedCommand& cmd)
{
	if (!isValid(cmd))
		return false;
	std::map<std::string, Channel *>::iterator	it = this->_channelMap.find(cmd[1]);
	if (it == this->_channelMap.end())
		return false;
	if (it->second->isOperator(*client) && it->isClient(cmd[2]))
		return it->second->removeClient(cmd[2], cmd[3]);
	return false;
}