#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <liblightnvm.h>
#include <liblightnvm_cli.h>

int cmd_geo(struct nvm_cli *cli)
{
	nvm_cli_info_pr("Device information -- nvm_geo_pr");
	nvm_geo_pr(nvm_dev_get_geo(cli->args.dev));

	return 0;
}

int cmd_info(struct nvm_cli *cli)
{
	nvm_cli_info_pr("Device information -- nvm_dev_pr");
	nvm_dev_pr(cli->args.dev);

	return 0;
}

/**
 * Command-line interface (CLI) boiler-plate
 */

/* Define commands */
static struct nvm_cli_cmd cmds[] = {
	{"geo", cmd_geo, NVM_CLI_ARG_DEV_PATH, NVM_CLI_OPT_DEFAULT},
	{"info", cmd_info, NVM_CLI_ARG_DEV_PATH, NVM_CLI_OPT_DEFAULT},
};

/* Define the CLI */
static struct nvm_cli cli = {
	.title = "NVM Device (nvm_dev_*)",
	.descr_short = "Retrieve device information",
	.cmds = cmds,
	.ncmds = sizeof(cmds) / sizeof(cmds[0]),
};

/* Initialize and run */
int main(int argc, char **argv)
{
	int res = 0;

	if (nvm_cli_init(&cli, argc, argv) < 0) {
		perror("# FAILED");
		return 1;
	}

	res = nvm_cli_run(&cli);

	nvm_cli_destroy(&cli);

	return res;
}
