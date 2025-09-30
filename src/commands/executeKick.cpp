
static bool	isValid(const ParsedCommand& cmd)
{
	int size = cmd.params.size();
	if (size != 4)
		return false;
	return true;
}

bool	Server::executeKick(Client* client, const ParsedCommand& cmd)
{
	if (!isValid)
		return false;
	for (Channel ch : this->_channelList)
	{
		if (cmd[1] == ch->getName())
		{
			if (isOperator(*client))
				ch->removeClient(cmd[2]);
			return true;
		}
	}
	return false;
}