#pragma once
#pragma once

namespace kmint
{
	namespace pigisland
	{
			template<typename... Args>
			class message
			{
				typedef std::function<void(Args...)> Callback;
				std::vector<Callback> callbacks;

			public:
				message() = default;
				void addCallback(const Callback callback)
				{
					callbacks.push_back(callback);
				}

				void removeCallback(Callback callback)
				{
					std::vector<int>::iterator position = std::find(callbacks.begin(), callbacks.end(), callback);
					if (position != callbacks.end())
						callbacks.erase(position);
				}

				void popCallback() {
					callbacks.pop_back();
				}

				void send(Args... args) const
				{
					for (auto callback : callbacks)
					{
						callback(args...);
					}
				}
			};
		}
}