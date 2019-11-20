#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"

extern void Um_write_sequence(FILE *output, Seq_T instructions);

extern void emit_halt_test(Seq_T instructions);
extern void emit_verbose_halt_test(Seq_T instructions);
extern void emit_add_test(Seq_T instructions);
extern void emit_load_val_test(Seq_T instructions);
extern void emit_add_test2(Seq_T instructions);
extern void emit_print_digit_test(Seq_T instructions);
extern void emit_conditional_move_test(Seq_T instructions);
extern void emit_nand_test(Seq_T instructions);
extern void emit_mult_test(Seq_T instructions);
extern void emit_div_test(Seq_T instructions);
extern void emit_div_zero_test(Seq_T instructions);
extern void emit_input_test(Seq_T instructions);
extern void emit_output_test(Seq_T instructions);
extern void emit_fancy_nand_test(Seq_T instructions);
extern void emit_segmented_store_and_load_test(Seq_T instructions);
extern void emit_unmap_twice_test(Seq_T instructions);
extern void emit_more_loads_test(Seq_T instructions);
extern void emit_many_instructions_test(Seq_T instructions);
extern void emit_oob_test(Seq_T instructions);

/* CHECK */
extern void emit_segmented_store_test(Seq_T instructions);
extern void emit_segmented_load_test(Seq_T instructions);
extern void emit_unmap_twice_test(Seq_T instructions);
extern void emit_load_program_test(Seq_T instructions);
extern void emit_map_unmap_test(Seq_T instructions);
extern void emit_map_unmap_loop_test(Seq_T instructions);

/* weird */
extern void emit_map_segment_test(Seq_T instructions);
extern void emit_unmap_segment_test(Seq_T instructions);

/* The array `tests` contains all unit tests for the lab. */
static struct test_info
{
        const char *name;
        const char *test_input; /* NULL means no input needed */
        const char *expected_output;
        /* writes instructions into sequence */
        void (*emit_test)(Seq_T stream);
} tests[] = {
    {"halt", NULL, "", emit_halt_test},
    {"halt-verbose", NULL, "Bad!\n", emit_verbose_halt_test},
    {"add", NULL, "e\n", emit_add_test},
    {"add2", "23", "e\n", emit_add_test2},
    {"load-val", NULL, "d\n", emit_load_val_test},
    {"seg-store", NULL, "", emit_segmented_store_test},
    {"seg-load", NULL, "", emit_segmented_load_test},
    {"mult", NULL, "d\n", emit_mult_test},
    {"div", NULL, "?\n", emit_div_test},
    {"div_by_zero", NULL, "", emit_div_zero_test},
    {"load-prog", NULL, "4\n", emit_load_program_test},
    {"print-digit", NULL, "6\n", emit_print_digit_test},
    {"conditional-move", NULL, "3\n5\n", emit_conditional_move_test},
    {"nand", NULL, "a\n", emit_nand_test},
    {"input", "3", "3\n", emit_input_test},
    {"output", NULL, "3\n", emit_output_test},
    {"map-seg", NULL, "1\n", emit_map_segment_test},     /* unprintable ASCII */
    {"unmap-seg", NULL, "1\n", emit_unmap_segment_test}, /* unprintable ASCII */
    {"fancy-nand", "5", "5\n", emit_fancy_nand_test},
    {"seg-store-load", NULL, "a\n", emit_segmented_store_and_load_test},
    {"map-unmap-ez", NULL, "1\n", emit_map_unmap_test},
    {"unmap2", NULL, "", emit_unmap_twice_test},
    {"more-load-seg", NULL, "1\n0\n", emit_more_loads_test},
    {"map-unmap-loop", NULL, "12", emit_map_unmap_loop_test},
    {"many-instruct", NULL, "1\n", emit_many_instructions_test}
};

#define NTESTS (sizeof(tests) / sizeof(tests[0]))

/*
 * open file 'path' for writing, then free the pathname;
 * if anything fails, checked runtime error
 */
static FILE *open_and_free_pathname(char *path);

/*
 * if contents is NULL or empty, remove the given 'path',
 * otherwise write 'contents' into 'path'.  Either way, free 'path'.
 */
static void write_or_remove_file(char *path, const char *contents);

static void write_test_files(struct test_info *test);

int main(int argc, char *argv[])
{
        bool failed = false;
        if (argc == 1)
                for (unsigned i = 0; i < NTESTS; i++)
                {
                        printf("***** Writing test '%s'.\n", tests[i].name);
                        write_test_files(&tests[i]);
                }
        else
                for (int j = 1; j < argc; j++)
                {
                        bool tested = false;
                        for (unsigned i = 0; i < NTESTS; i++)
                                if (!strcmp(tests[i].name, argv[j]))
                                {
                                        tested = true;
                                        write_test_files(&tests[i]);
                                }
                        if (!tested)
                        {
                                failed = true;
                                fprintf(stderr,
                                        "***** No test named %s *****\n",
                                        argv[j]);
                        }
                }
        return failed; /* failed nonzero == exit nonzero == failure */
}

static void write_test_files(struct test_info *test)
{
        FILE *binary = open_and_free_pathname(Fmt_string("%s.um", test->name));
        Seq_T instructions = Seq_new(0);
        test->emit_test(instructions);
        Um_write_sequence(binary, instructions);
        Seq_free(&instructions);
        fclose(binary);

        write_or_remove_file(Fmt_string("%s.0", test->name),
                             test->test_input);
        write_or_remove_file(Fmt_string("%s.1", test->name),
                             test->expected_output);
}

static void write_or_remove_file(char *path, const char *contents)
{
        if (contents == NULL || *contents == '\0')
        {
                remove(path);
        }
        else
        {
                FILE *input = fopen(path, "wb");
                assert(input != NULL);

                fputs(contents, input);
                fclose(input);
        }
        free(path);
}

static FILE *open_and_free_pathname(char *path)
{
        FILE *fp = fopen(path, "wb");
        assert(fp != NULL);

        free(path);
        return fp;
}
