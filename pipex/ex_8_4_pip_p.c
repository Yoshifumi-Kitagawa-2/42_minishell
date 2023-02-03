////pip_p.c///////
/*
親子関係にあるプロセスのプロセス間通信

実行結果
./pip_p                   
apple
apple <-> 5
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() 
{
	pid_t	pid;
	char	line[256],rfd[5], wfd[5], word[256];
	int		n, fd[2], st, ret1, ret2;

	ret1 = pipe(fd); //バッファへの読み取り用パイプ, 書き込み用パイプを開く。
	if (ret1 < 0){
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	snprintf(rfd, sizeof(rfd), "%d", fd[0]);
	snprintf(wfd, sizeof(wfd), "%d", fd[1]); //int型のファイルディスクリプタをchar型の変換

	fgets(line, sizeof(line), stdin);
	ret1 = sscanf(line, "%s", word);

	if (ret1 > 0)
	{
		pid = fork(); //子プロセス生成
		if (pid == 0){
			ret2 = execl("pip_c", "pip_c", rfd, wfd, NULL);
			if (ret2 < 0)
				exit(EXIT_FAILURE);
		}
		write(fd[1], word, sizeof(word)); //バッファへ文字列の書き込み
		wait(&st); //子プロセスの終了まで待機

		read(fd[0], &n, sizeof(n));
		fprintf(stderr, "%s <-> %d\n", word, n);
		close(fd[0]);
		close(fd[1]); //ファイルディスクリプタをクローズ
	}
	return (0);
}