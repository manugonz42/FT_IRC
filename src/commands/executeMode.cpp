#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"


static int	countRequiredParams(const std::string& modeString)
{
	int		paramCount = 0;
	bool	adding = true;

	int size = modeString.length();

	for (int i = 0; i < size; i++)
	{
		char c = modeString[i];
		if (c == '+')
			adding = true;
		else if (c == '-')
			adding = false;
		else if (c == 'o')
			paramCount++;
		else if (c == 'k' && adding)
			paramCount++;
		else if (c == 'l' && adding)
			paramCount++;
	}

	return paramCount;
}


bool	Server::executeMode(Client *client, const ParsedCommand &cmd)
{
	int	numParams = cmd.params.size();
	if (numParams < 2)
	{
		// not enough parameters
		return true;
	}
	if (cmd.params[1][0] == '#')
	{
		//Para futuro saneamiento, hacer estas comprobaciones en una funcion externa
		
		std::map<std::string, Channel *>::iterator	it = _channelMap.find(cmd.params[1]);
		if (it == _channelMap.end())
		{
			//error no such channel name
			return true;
		}

		if (numParams < 3)
		{
			// Show current channel modes
			return true; 	
		}

		int	requiredParams = countRequiredParams(cmd.params[2]);
		int	availableParams = numParams - 3;

		if (!it->second->isOperator(*client))
		{
			//error you are not an operator
			return true;
		}

		if (availableParams < requiredParams)
		{
			//error not enough parameters
			return true;
		}

		int 		paramIt = 3;
		int 		size = cmd.params[2].length();
		bool		adding = true;
		std::string	modeChanges = "";
		std::string	modeParams = "";
		for (int i = 0; i < size; i++)
		{
			char c = cmd.params[2][i];
			switch (c)
			{
				case '+': 
					adding = true;
					if (modeChanges.empty() || *modeChanges.rbegin() != '+')
						modeChanges += c;
					break;
				case '-': 
					adding = false; 
					if (modeChanges.empty() || *modeChanges.rbegin() != '-')
						modeChanges += c;
					break;
				case 'o':
					if (adding)
					{
						if (it->second->makeOperator(cmd.params[paramIt]))
						{
							modeChanges += c;
							if (!modeParams.empty()) modeParams += " ";
							modeParams += cmd.params[paramIt];
						}
					}
					else
					{
						if (it->second->removeOperator(cmd.params[paramIt]))
						{
							modeChanges += c;
							if (!modeParams.empty()) modeParams += " ";
							modeParams += cmd.params[paramIt];
						}
					}
						paramIt++;
					break;
				case 'k':
					modeChanges += c;
					if (adding)
					{
						it->second->changeKey(cmd.params[paramIt]);
						if (!modeParams.empty()) modeParams += " ";
						modeParams += cmd.params[paramIt];
						paramIt++;
					}
					else
						it->second->changeKey("");

					break;
				case 'l':
					modeChanges += c;
					if (adding)
					{
						it->second->changeLimit(cmd.params[paramIt]);
						if (!modeParams.empty()) modeParams += " ";
						modeParams += cmd.params[paramIt];
						paramIt++;
					}
					else
						it->second->changeLimit("-1");

					break;
				case 'i':
					modeChanges += c;
					if(adding)
						it->second->inviteOnly(true);
					else
						it->second->inviteOnly(false);
					break;
				case 't':
					modeChanges += c;
					if(adding)
						it->second->topicRestriction(true);
					else
						it->second->topicRestriction(false);
					break;
				default:
					break;
			}	
		}

		if (!modeChanges.empty())
		{
			std::string prefix = ":" + client->getField("NICK") + "!user@host ";
			std::string modeMsg = "MODE " + cmd.params[1] + " " + modeChanges;
			if (!modeParams.empty())
				modeMsg += " " + modeParams;
			return it->second->sendMessage(NULL, modeMsg, prefix);
		}
	}
	else
	{
		//mode personal
		(void)client;
	}
	
	return true;
}