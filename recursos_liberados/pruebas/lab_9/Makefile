CC     = gcc
CFLAGS = -O2
OUTDIR = outputs
DBDIR  = db
IMGDIR = img

SRCS = schedulling_naive.c \
       schedulling_weighted_dp.c \
       schedulling_weighted_greedy.c \
       schedulling_greedy.c

OUTS = $(OUTDIR)/schedulling_naive.out \
       $(OUTDIR)/schedulling_weighted_dp.out \
       $(OUTDIR)/schedulling_weighted_greedy.out \
       $(OUTDIR)/schedulling_greedy.out

all: $(OUTDIR) $(DBDIR) $(IMGDIR) $(OUTS)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(DBDIR):
	mkdir -p $(DBDIR)

$(IMGDIR):
	mkdir -p $(IMGDIR)

$(OUTDIR)/%.out: %.c | $(OUTDIR) $(DBDIR)
	$(CC) $(CFLAGS) -o $@ $<

run: all
	./$(OUTDIR)/schedulling_naive.out
	./$(OUTDIR)/schedulling_weighted_dp.out
	./$(OUTDIR)/schedulling_weighted_greedy.out
	./$(OUTDIR)/schedulling_greedy.out

run-1: $(OUTDIR)/schedulling_naive.out $(DBDIR)
	./$(OUTDIR)/schedulling_naive.out

run-2: $(OUTDIR)/schedulling_weighted_dp.out $(DBDIR)
	./$(OUTDIR)/schedulling_weighted_dp.out

run-3: $(OUTDIR)/schedulling_weighted_greedy.out $(DBDIR)
	./$(OUTDIR)/schedulling_weighted_greedy.out

run-4: $(OUTDIR)/schedulling_greedy.out $(DBDIR)
	./$(OUTDIR)/schedulling_greedy.out

plots: $(IMGDIR)
	gnuplot plots/plot_naive_solo.gp
	gnuplot plots/plot_weighted_comparison.gp
	gnuplot plots/plot_all_comparison.gp
	gnuplot plots/plot_greedy_comparison.gp

clean:
	rm -f $(OUTDIR)/* $(DBDIR)/* $(IMGDIR)/*

.PHONY: all run run-1 run-2 run-3 run-4 plots clean
