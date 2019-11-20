/*
 * umlab.c
 * 
 * The functions defined in this lab should be linked against umlabwrite.c
 * to produce a unit test writing program. Any additional functions and unit
 * tests written for the lab go here.
 */

#include <stdint.h>
#include <stdio.h>

#include <assert.h>
#include <seq.h>
#include <bitpack.h>

const int REGISTER = 3;
const int INSTRUCT = 4;

typedef uint32_t Um_instruction;
typedef enum Um_opcode
{
	CMOV = 0,
	SLOAD,
	SSTORE,
	ADD,
	MUL,
	DIV,
	NAND,
	HALT,
	ACTIVATE,
	INACTIVATE,
	OUT,
	IN,
	LOADP,
	LV
} Um_opcode;

/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{

	Um_instruction curr = 0;
	uint32_t ra_new = (uint32_t)ra;
	uint32_t rb_new = (uint32_t)rb;
	uint32_t rc_new = (uint32_t)rc;

	curr = Bitpack_newu(curr, INSTRUCT, 28, op);
	curr = Bitpack_newu(curr, REGISTER, 6, ra_new);
	curr = Bitpack_newu(curr, REGISTER, 3, rb_new);
	curr = Bitpack_newu(curr, REGISTER, 0, rc_new);

	return curr;
}

Um_instruction loadval(unsigned ra, unsigned val)
{
	Um_instruction curr = 0;
	curr = Bitpack_newu(curr, INSTRUCT, 28, 13);
	curr = Bitpack_newu(curr, REGISTER, 25, ra);
	curr = Bitpack_newu(curr, 25, 0, val);
	fprintf(stderr, "%u\n", curr);
	return curr;
}

/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void)
{
	return three_register(HALT, 0, 0, 0);
}

static inline Um_instruction segmented_store(int ra, int rb, int rc)
{
	return three_register(SSTORE, ra, rb, rc);
}

static inline Um_instruction segmented_load(int ra, int rb, int rc)
{
	return three_register(SLOAD, ra, rb, rc);
}

static inline Um_instruction mult(int ra, int rb, int rc)
{
	return three_register(MUL, ra, rb, rc);
}

static inline Um_instruction div(int ra, int rb, int rc)
{
	return three_register(DIV, ra, rb, rc);
}

static inline Um_instruction load_program(int rb, int rc)
{
	return three_register(LOADP, 0, rb, rc);
}

static inline Um_instruction c_move(int ra, int rb, int rc)
{
	return three_register(CMOV, ra, rb, rc);
}

static inline Um_instruction nand(int ra, int rb, int rc)
{
	return three_register(NAND, ra, rb, rc);
}

static inline Um_instruction map(int rb, int rc)
{
	return three_register(ACTIVATE, 0, rb, rc);
}

static inline Um_instruction unmap(int rc)
{
	return three_register(INACTIVATE, 0, 0, rc);
}

typedef enum Um_register
{
	r0 = 0,
	r1,
	r2,
	r3,
	r4,
	r5,
	r6,
	r7
} Um_register;

static inline Um_instruction add(Um_register a, Um_register b, Um_register c)
{
	return three_register(ADD, a, b, c);
}

static inline Um_instruction input(Um_register c)
{
	return three_register(IN, 0, 0, c);
}
static inline Um_instruction output(Um_register c)
{
	return three_register(OUT, 0, 0, c);
}

/* Functions for working with streams */
static inline void emit(Seq_T stream, Um_instruction inst)
{
	assert(sizeof(inst) <= sizeof(uintptr_t));
	Seq_addhi(stream, (void *)(uintptr_t)inst);
}

static void endian_fix(FILE *output, Um_instruction elem)
{
	const int width = 8;

	for (int i = 24; i >= 0; i -= 8)
	{
		putc(Bitpack_getu(elem, width, i), output);
	}
}
extern void Um_write_sequence(FILE *output, Seq_T stream)
{
	Seq_T curr_stream = stream;
	Um_instruction elem; // a 32 bit Um instruction
	while (Seq_length(curr_stream) != 0)
	{
		elem = (Um_instruction)(uintptr_t)Seq_remlo(curr_stream);
		endian_fix(output, elem);
	}
}

/* Unit tests for the UM */

// need to make .um files for taking from standard in ("input")

void emit_halt_test(Seq_T stream)
{
	emit(stream, halt());
}

void emit_add_test(Seq_T stream)
{
	emit(stream, loadval(r2, 50));
	emit(stream, loadval(r3, 51));
	emit(stream, add(r1, r2, r3));
	emit(stream, output(r1));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

// stdin
void emit_add_test2(Seq_T stream)
{
	emit(stream, input(r2));
	emit(stream, input(r3));
	emit(stream, add(r1, r2, r3));
	emit(stream, output(r1));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

void emit_mult_test(Seq_T stream)
{
	emit(stream, loadval(r2, 10));
	emit(stream, loadval(r3, 10));
	emit(stream, mult(r1, r2, r3));
	emit(stream, output(r1));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

void emit_div_test(Seq_T stream)
{
	emit(stream, loadval(r2, 126));
	emit(stream, loadval(r3, 2));
	emit(stream, div(r1, r2, r3));
	emit(stream, output(r1));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

void emit_div_zero_test(Seq_T stream)
{
	emit(stream, loadval(r2, 126));
	emit(stream, loadval(r3, 0));
	emit(stream, div(r1, r2, r3));
	emit(stream, output(r1));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

void emit_load_val_test(Seq_T stream)
{

	emit(stream, loadval(r1, 100));
	emit(stream, output(r1));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

/* CHECK */
void emit_segmented_store_test(Seq_T stream)
{
	emit(stream, loadval(r1, 25));
	emit(stream, loadval(r2, 20));
	emit(stream, loadval(r3, 15));
	emit(stream, segmented_store(r1, r2, r3)); // stores what's in r3 into mem
	emit(stream, output(r3));
	emit(stream, segmented_load(r4, r2, r3));
	emit(stream, output(r4));
	emit(stream, halt());
}

/* CHECK
	these are indexing, so have to be small values for indices */
void emit_segmented_load_test(Seq_T stream)
{
	// emit(stream, loadval(r1, 1));
	emit(stream, loadval(r2, 0));
	emit(stream, loadval(r3, 3));
	emit(stream, segmented_load(r1, r2, r3));
	emit(stream, output(r1)); // this instruction will be in r1
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

void emit_segmented_store_and_load_test(Seq_T stream)
{
	emit(stream, loadval(r1, 'a'));
	emit(stream, segmented_store(0, 0, r1));
	emit(stream, segmented_load(r2, 0, 0));
	emit(stream, output(r2));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

void emit_verbose_halt_test(Seq_T stream)
{
	emit(stream, loadval(r1, 'B'));
	emit(stream, output(r1));
	emit(stream, loadval(r1, 'a'));
	emit(stream, output(r1));
	emit(stream, loadval(r1, 'd'));
	emit(stream, output(r1));
	emit(stream, loadval(r1, '!'));
	emit(stream, output(r1));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

void emit_print_digit_test(Seq_T stream)
{
	emit(stream, loadval(r1, 48));
	emit(stream, loadval(r2, 6));
	emit(stream, add(r3, r1, r2));
	emit(stream, output(r3));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

/* CHECK */
/* should do a more complicated test of this once we know
	map_segment works properly */
/* this is broken bc endless loop 
	will fix */
void emit_load_program_test(Seq_T stream)
{
	emit(stream, loadval(r2, 0));
	emit(stream, loadval(r3, 5));
	emit(stream, load_program(r2, r3));
	emit(stream, output(r3)); 
	emit(stream, loadval(r1, 5));
	emit(stream, loadval(r1, 4));
	emit(stream, loadval(r4, 48));
	emit(stream, add(r1, r1, r4));
	emit(stream, output(r1));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

void emit_more_loads_test(Seq_T stream)
{
	emit(stream, loadval(r1, 5)); // specifying new segment of __
	emit(stream, map(r2, r1)); // r2 is index, should be 1
	emit(stream, loadval(r3, 48)); 
	emit(stream, add(r3, r2, r3));
	emit(stream, output(r3));
	emit(stream, loadval(r4, '\n'));
	emit(stream, output(r4)); // making sure index is 1

	emit(stream, loadval(r1, 1));
	emit(stream, loadval(r2, 2));
	emit(stream, loadval(r3, 3));
	emit(stream, loadval(r4, 4));
	emit(stream, loadval(r7, 0));
	emit(stream, loadval(r6, 5));
	emit(stream, segmented_load(r5, r7, r2));
	emit(stream, segmented_store(r1, r7, r5));
	emit(stream, segmented_load(r5, r7, r4));
	emit(stream, segmented_store(r1, r1, r5));
	emit(stream, segmented_load(r5, r7, r6));
	emit(stream, segmented_store(r1, r2, r5));

	emit(stream, loadval(r2, 6));

	emit(stream, segmented_load(r5, r7, r2));
	emit(stream, segmented_store(r1, r3, r5));

	emit(stream, loadval(r2, 28));

	emit(stream, segmented_load(r5, r7, r2));
	emit(stream, segmented_store(r1, r4, r5));
	
	emit(stream, loadval(r7, 1));
	emit(stream, loadval(r6, 0));
	emit(stream, load_program(r7, r6));

	emit(stream, halt());
}

void emit_conditional_move_test(Seq_T stream)
{
	emit(stream, loadval(r1, 3));
	emit(stream, loadval(r2, 5));
	emit(stream, loadval(r3, 0));
	emit(stream, c_move(r1, r2, r3));
	emit(stream, loadval(r4, 48));
	emit(stream, add(r1, r1, r4));
	emit(stream, output(r1));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, loadval(r1, 3));
	emit(stream, loadval(r3, 1));
	emit(stream, c_move(r1, r2, r3));
	emit(stream, add(r1, r1, r4));
	emit(stream, output(r1));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

/**** should make harder test **/
void emit_nand_test(Seq_T stream)
{
	emit(stream, loadval(r2, 'a'));
	emit(stream, loadval(r3, 'a'));
	emit(stream, nand(r1, r2, r3));
	emit(stream, nand(r1, r1, r1));
	emit(stream, output(r1));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

/* this is basically our initial nand test */
void emit_fancy_nand_test(Seq_T stream)
{
	emit(stream, input(r1));
	emit(stream, nand(r2, r1, r1));
	emit(stream, nand(r2, r2, r2));
	emit(stream, output(r2));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

void emit_input_test(Seq_T stream)
{
	emit(stream, input(r3));
	emit(stream, output(r3));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

void emit_output_test(Seq_T stream)
{
	emit(stream, loadval(r1, '3'));
	emit(stream, output(r1));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

/*
	Mapping tests: 
	- mapping to the same location 1+ times
	- mapping, unmapping, mapping back to same place
	- unmapping twice 

*/
void emit_map_segment_test(Seq_T stream)
{
	emit(stream, loadval(r3, 5));
	emit(stream, map(r2, r3));
	emit(stream, loadval(r4, 48));
	emit(stream, add(r2, r2, r4));
	emit(stream, output(r2)); /* this gives new index of new seg */
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

void emit_unmap_segment_test(Seq_T stream)
{
	emit(stream, loadval(r3, 5));
	emit(stream, map(r2, r3));
	emit(stream, unmap(r2));
	emit(stream, map(r2, r3));
	emit(stream, loadval(r4, 48));
	emit(stream, add(r2, r2, r4)); // adds 48 to create printable ascii
	emit(stream, output(r2));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

/* map, unmap, then map */
void emit_map_unmap_test(Seq_T stream)
{
	emit(stream, loadval(r3, 7));
	emit(stream, map(r2, r3)); // creates new seg length 7
	emit(stream, unmap(r2));   // unmaps a segment
	emit(stream, map(r2, r3)); // maps a segment of length 7 at same index
	emit(stream, loadval(r4, 48));
	emit(stream, add(r2, r2, r4));
	emit(stream, output(r2));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

void emit_unmap_twice_test(Seq_T stream)
{
	emit(stream, loadval(r3, 5));
	emit(stream, map(r2, r3));
	emit(stream, unmap(r2));
	// emit(stream, map(r2, r3));
	emit(stream, unmap(r2));
	emit(stream, loadval(r4, 48));
	emit(stream, add(r2, r2, r4));
	emit(stream, output(r2));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
	emit(stream, halt());
}

// this test should just interate once, can have a variation
// that loops through many times that has 1-to-1 ration of mapping/unmap
// void emit_map_unmap_bigger_test(Seq_T stream)
// {
// 	emit(stream, loadval(r1, 5));
// 	emit(stream, loadval(r2, 7));
// 	emit(stream, loadval(r3, 2));
// 	emit(stream, map(r4, r1)); // map segment length 5
// 	emit(stream, map(r5, r2));	// map segment length 7
// 	emit(stream, map(r6, r3)); // map segment length 2
// 	emit(stream, map(r1, r2)); // maps segment length 7

// 	emit(stream, unmap(r4));
// 	emit(stream, unmap(r5));
// 	emit(stream, unmap(r6));
// 	emit(stream, unmap(r1)); // frees up segment at r1
// 	emit(stream, map(r1, r2));

// 	emit(stream, loadval(r7, 48));
// 	emit(stream, add(r1, r1, r7));
// 	emit(stream, add(r6, r6, r7));
// 	emit(stream, add(r4, r4, r7));
// 	emit(stream, add(r5, r5, r7));
// 	emit(stream, output(r1));	// 4, 52
// 	emit(stream, output(r4));   // 1, 49
// 	emit(stream, output(r5));   // 2, 50
// 	emit(stream, output(r6));   // 3, 51

// 	emit(stream, halt());
// }

void emit_map_unmap_loop_test(Seq_T stream)
{
	for(int i = 0; i < 5000; i++){
		emit(stream, loadval(r1, 5));
		emit(stream, loadval(r3, 3));
		emit(stream, map(r2, r1));
		emit(stream, map(r4, r3));
		emit(stream, unmap(r2));
		emit(stream, unmap(r4));
	}
	emit(stream, loadval(r7, 48));
	emit(stream, add(r2, r2, r7));
	emit(stream, add(r4, r4, r7));
	emit(stream, output(r2));
	emit(stream, output(r4));
	emit(stream, halt());

}



void emit_many_instructions_test(Seq_T stream)
{
	emit(stream, loadval(r1, 0));
	emit(stream, loadval(r2, 49));
	for (int i = 0; i < 500000; i++) {
		emit(stream, add(r2, r1, r2));
	}
	emit(stream, output(r2));
	emit(stream, loadval(r3, '\n'));
	emit(stream, output(r3));
	emit(stream, halt());
}

/***** ADD TO README *****/
void emit_oob_test(Seq_T stream)
{
	emit(stream, loadval(r1, 300));
	emit(stream, output(r1));
	emit(stream, halt());
}




/* TEST MAP/UNMAP IN BIG LOOP */

/**
 * map new segment
 * use segmented store/load to put in new instructions
 * use loadprogram
 * 
 * 
 * 
 */