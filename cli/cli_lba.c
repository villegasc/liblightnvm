/**
 * lba - CLI wrapping nvm_lba_*
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <liblightnvm_cli.h>

int cmd_pwrite(struct nvm_cli *cli)
{
	struct nvm_cli_cmd_args *args = &cli->args;
	struct nvm_ret ret = {0,0};
	ssize_t err = 0;
	char *buf;

	size_t count = args->dec_vals[0], offset = args->dec_vals[1];

	buf = nvm_buf_alloc(args->geo, count);
	if (!buf) {
		perror("nvm_buf_alloc");
		return ENOMEM;
	}
	nvm_buf_fill(buf, count);

	printf("** nvm_lba_pwrite(...): count(%lu), offset(%ld)\n",
	       count, offset);

	err = nvm_lba_pwrite(args->dev, buf, count, offset);
	if (err) {
		perror("nvm_lba_pwrite");
		nvm_ret_pr(&ret);
	}

	free(buf);

	return err ? 1 : 0;
}

int cmd_pread(struct nvm_cli *cli)
{
	struct nvm_cli_cmd_args *args = &cli->args;
	struct nvm_ret ret = {0,0};
	ssize_t err = 0;
	char *buf;

	size_t count = args->dec_vals[0], offset = args->dec_vals[1];

	buf = nvm_buf_alloc(args->geo, count);
	if (!buf) {
		perror("nvm_buf_alloc");
		return ENOMEM;
	}

	printf("** nvm_lba_pread(...): count(%lu), offset(%ld)\n",
	       count, offset);

	err = nvm_lba_pread(args->dev, buf, count, offset);
	if (err) {
		perror("nvm_lba_pread");
		nvm_ret_pr(&ret);
	}

	if (cli->evars.buf_pr)
		nvm_buf_pr(buf, count);

	free(buf);

	return err ? 1 : 0;
}

//
// Remaining code is CLI boiler-plate
//

static struct nvm_cli_cmd cmds[] = {
	{"pwrite", cmd_pwrite, NVM_CLI_ARG_COUNT_OFFSET, NVM_CLI_OPT_DEFAULT},
	{"pread", cmd_pread, NVM_CLI_ARG_COUNT_OFFSET, NVM_CLI_OPT_DEFAULT},
};

/* Define the CLI */
static struct nvm_cli cli = {
	.title = "NVM logical-block-address (nvm_lba_*)",
	.descr_short = NULL,
	.cmds = cmds,
	.ncmds = sizeof(cmds) / sizeof(cmds[0]),
};

/* Initialize and run */
int main(int argc, char **argv)
{
	if (nvm_cli_init(&cli, argc, argv) < 0) {
		perror("FAILED");
		return 1;
	}

	if (nvm_cli_run(&cli) < 0)
		perror(cli.cmd.name);
	
	nvm_cli_destroy(&cli);

	return 0;
}
