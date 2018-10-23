#include <git2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"

struct opts {
	int no_options;
	int quiet;
	int bare;
	int initial_commit;
	uint32_t shared;
	const char *template;
	const char *gitdir;
	const char *dir;
};

static void create_initial_commit (git_repository *repo){
  git_signature *sig;
  git_index *index;
  git_oid tree_id, commit_id;
  git_tree *tree;
  if (git_signature_default(&sig, repo) < 0)
    fprintf(stderr, "Unable to create a commit signature.",
      "Perhaps 'user.name' and 'user.email' are not set");
  if (git_repository_index(&index, repo) < 0)
    fprintf(stderr, "Could not open repository index", NULL);

  if (git_index_write_tree(&tree_id, index) < 0)
    fprintf(stderr, "Unable to write initial tree from index", NULL);

  git_index_free(index);if (git_tree_lookup(&tree, repo, &tree_id) < 0)
  fprintf(stderr, "Could not look up initial tree", NULL);
  if (git_commit_create_v(&commit_id, repo, "HEAD", sig, sig,
			NULL, "Initial commit", tree, 0) < 0)
    fprintf(stderr, "Could not create the initial commit", NULL);git_tree_free(tree);
  git_signature_free(sig);
}

int main(int argc, char *argv[]){
  git_repository *repo = NULL;
  struct opts o = { 1, 0, 0, 0, GIT_REPOSITORY_INIT_SHARED_UMASK, 0, 0, 0 };

  git_libgit2_init();
  git_repository_init(&repo, "/tmp/aa", 0);
  o.dir = git_repository_path(repo);
  printf("Initialized empty Git repository in %s\n", o.dir);
  


  create_initial_commit(repo);
  //  printf("Created empty initial commit\n");
  git_repository_free(repo);
  git_libgit2_shutdown();

  return 0;
}


