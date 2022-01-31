#
# This make file will compile .cc to .exe using g++.
# NOTE: A line starting with '#' is a comment.
#
# To make use of this file, run in the current directory any of:
#   1) make -f gcc-makefile.mak
#   2) make -f gcc-makefile.mak clean
#   3) make -f gcc-makefile.mak all
#   (The make command will output all commands issued.)
#

# The next (optional) line explicitly sets the C++ compiler program name
# using a special variable called CXX.
CXX=g++

# The next (optional) line sets the C++ compiler options one wants to use
# using a special variable called CXXFLAGS.
CXXFLAGS=-std=c++11 -Wall -Wextra -Werror -O2 -march=broadwell

# The next line is the first rule --which also makes it the default rule.
# It tells make to try building the files ending with .exe.
all: \
	workers_iterations_gen.exe \
	future_iterations_gen.exe

# The next rule mentions a target called "clean". Intentionally there
# is no file that will exist called "clean". By convention, the clean
# deletes any generated files from previous invocations of make.
#
# NOTE: The lines that follow the first line of a rule must ALWAYS be
#       intended by ONE TAB character (not spaces!). For clean, this
#       will typically be a series of rm -f commands.
clean:
	rm -f workers_iterations_gen.exe future_iterations_gen.exe

# The next line tells make that .exe depends on .cpp.
# Make is smart: it will look at the timestamp of .exe and if it is
# older than .cpp (or if .exe does not exist), it will perform
# the actions immediately following it. This is useful because if you've
# modified the source code (and saved it), it will recompile the program
# --otherwise it won't.
#
# NOTE: $@ refers to the "target", i.e., .exe in this instance.
#       $< refers to the first "dependent", i.e., .cpp in this instance.
workers_iterations_gen.exe: workers_iteration_intervals_gen.cc generator.cc iofile.cc
	$(CXX) $(CXXFLAGS) -o $@ $<

future_iterations_gen.exe: future_iteration_intervals_gen.cc generator.cc iofile.cc
	$(CXX) $(CXXFLAGS) -o $@ $<
