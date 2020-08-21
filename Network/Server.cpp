#include "Server.hpp"

namespace swl
{
	Server::Server()
	{

	}
	bool Server::isWork() const
	{
		return work;
	}
	TCPServer::TCPServer()
	{
	}
	void TCPServer::run(const IPEndpoint& ip, uint16_t port)
	{
		if (work) return;
		work = true;
		socket.bind(ip, port);
		selector.add(socket);
		socket.listen();
		std::thread([&]()
			{
				uint32_t id{};
				Packet packet;
				uint32_t client_id = 1;
				Socket::Status status;
				while (work)
				{
					if (selector.wait())
					{
						if (selector.isReady(socket))
						{
							clients.push_back(std::make_pair(TCPSocket(), client_id++));
							socket.accept(clients.back().first);
							selector.add(clients.back().first);
						}
						else
						{
							for (auto& client : clients)
							{
								packet.clear();
								if (selector.isReady(client.first))
								{
									status = client.first.receiveAll((void*)&id, 4);
									status = client.first.receive(packet);
									if (status != Socket::Done)
									{
										selector.remove(client.first);
										clients.erase(std::find_if(clients.begin(), clients.end(), [&](std::pair<TCPSocket, uint32_t>& clientt)
											{
												if (clientt.second == client.second)
													return true;
												return false;
											}));
										clients.reserve(clients.size());
										continue;
									}
									if (packet.getSize() > 0)
									{
										auto recipient = std::find_if(clients.begin(), clients.end(), [&](std::pair<TCPSocket, uint32_t>& client)
											{
												if (client.second == id)
													return true;
												return false;
											});
										if (recipient != clients.end())
										{
											(*recipient).first.sendAll((void*)&client.second, 4);
											(*recipient).first.send(packet);
										}
										else
										{
											for (auto& recipientt : clients)
											{
												if (recipientt.second != client.second)
												{
													recipientt.first.sendAll((void*)&client.second, 4);
													recipientt.first.send(packet);
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}).detach();
	}
	void TCPServer::stop()
	{
		work = false;
		socket.close();
	}
	UDPServer::UDPServer()
	{
	}
	void UDPServer::run(const IPEndpoint& ip, uint16_t port)
	{
		if (work) return;
		work = true;
		socket.bind(ip, port);
		selector.add(socket);
		clients.clear();
		std::thread([&]()
			{
				uint32_t id{};
				uint32_t client_id = 1;
				Packet packet;
				IPEndpoint ip;
				uint16_t port;
				Socket::Status status;
				while (work)
				{
					if (selector.wait())
					{
						if (selector.isReady(socket))
						{
							status = socket.receiveAll((void*)&id, 4, ip, port);
							status = socket.receive(packet, ip, port);
							//Находим отправителя пакета в clients
							auto sender = std::find_if(clients.begin(), clients.end(), [&](std::tuple<UDPSocket, IPEndpoint, uint16_t, uint32_t>& clientt)
								{
									if (std::get<1>(clientt) == ip && std::get<2>(clientt) == port)
										return true;
									return false;
								});
							//Если его там нету
							if (sender == clients.end())
								clients.push_back(std::make_tuple(UDPSocket(), ip, port, client_id++));
							//Если он там есть
							else
								if (status != Socket::Done)
								{
									std::get<0>(*sender).close();
									clients.erase(sender);
									continue;
								}
							if (packet.getSize() > 0)
							{
								//Находим того, кому надо отправить по id
								auto recipient = std::find_if(clients.begin(), clients.end(), [&](std::tuple<UDPSocket, IPEndpoint, uint16_t, uint32_t>& client)
									{
										if (std::get<3>(client) == id)
											return true;
										return false;
									});
								sender = std::find_if(clients.begin(), clients.end(), [&](std::tuple<UDPSocket, IPEndpoint, uint16_t, uint32_t>& clientt)
									{
										if (std::get<1>(clientt) == ip && std::get<2>(clientt) == port)
											return true;
										return false;
									});
								//Если есть получатель
								if (recipient != clients.end())
								{
									std::get<0>(*recipient).sendAll((const void*)&std::get<3>(*sender), 4, std::get<1>(*recipient), std::get<2>(*recipient));
									std::get<0>(*recipient).send(packet, std::get<1>(*recipient), std::get<2>(*recipient));
								}
								//Иначе кидаем всем, кроме отправителя
								else
								{
									for (auto& client : clients)
										if (std::get<1>(client) != ip || std::get<2>(client) != port) //Проверяем что client != sender
										{
											std::get<0>(client).sendAll((const void*)&std::get<3>(*sender), 4, std::get<1>(client), std::get<2>(client));
											std::get<0>(client).send(packet, std::get<1>(client), std::get<2>(client));
										}
								}
							}
						}
						/*else
						{
							for (auto& client : clients)
							{
								if (selector.isReady(std::get<0>(client)))
								{
									status = std::get<0>(client).receiveAll((void*)&id, 4, ip, port);
									status = std::get<0>(client).receive(packet, ip, port);
									if (status != Socket::Done)
									{
										selector.remove(std::get<0>(client));
										clients.erase(std::find_if(clients.begin(), clients.end(), [&](std::tuple<UDPSocket, IPEndpoint, uint16_t, uint32_t>& clientt)
											{
												if (std::get<3>(clientt) == std::get<3>(client))
												{
													return true;
												}
												return false;
											}));
										continue;
									}
									if (packet.getSize() > 0)
									{
										auto chosen_client = std::find_if(clients.begin(), clients.end(), [&](std::tuple<UDPSocket, IPEndpoint, uint16_t, uint32_t>& client)
											{
												if (std::get<3>(client) == id)
													return true;
												return false;
											});
										auto sender = std::find_if(clients.begin(), clients.end(), [&](std::tuple<UDPSocket, IPEndpoint, uint16_t, uint32_t>& client)
											{
												if (std::get<1>(client) == ip && std::get<2>(client) == port)
													return false;
												return true;
											});
										if (chosen_client != clients.end())
										{
											std::get<0>(*chosen_client).sendAll((const void*)&std::get<3>(*sender), 4, std::get<1>(*chosen_client), std::get<2>(*chosen_client));
											std::get<0>(*chosen_client).send(packet, std::get<1>(*chosen_client), std::get<2>(*chosen_client));
										}
										else
											for (auto& client : clients)
												if (std::get<1>(client) != ip || std::get<2>(client) != port)
												{
													std::get<0>(client).sendAll((const void*)&std::get<3>(*sender), 4, std::get<1>(client), std::get<2>(client));
													std::get<0>(client).send(packet, std::get<1>(client), std::get<2>(client));
												}
									}
								}
							}
						}*/
					}
				}
			}).detach();
	}
	void UDPServer::stop()
	{
		work = false;
		socket.close();
	}
}