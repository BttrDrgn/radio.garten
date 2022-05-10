#pragma once

struct process_t
{
	std::string title, arch;
	std::uint32_t pid;
};

class hook
{
public:
	static std::vector<process_t> processes;
	static std::vector<std::string> blacklist;
	static void load(std::uint32_t pid);
	static void get_procs();
};