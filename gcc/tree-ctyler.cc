/* Test pass
   Chris Tyler, Seneca Polytechnic College, 2024-11
   Modelled on tree-nrv.cc

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#define INCLUDE_MEMORY
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "tree.h"
#include "gimple.h"
#include "tree-pass.h"
#include "ssa.h"
#include "gimple-iterator.h"
#include "gimple-walk.h"
#include "internal-fn.h"
#include "gimple-pretty-print.h"

// Added headers:
#include "gimple-ssa.h"
#include "cgraph.h"
#include "attribs.h"
#include "pretty-print.h"
#include "tree-inline.h"
#include "intl.h"
#include "symtab.h"

// Included for dump_printf:
//#include "tree-pretty-print.h"
//#include "diagnostic.h"
//#include "dumpfile.h"
//#include "builtins.h"
//#include <stdlib.h>

// =================================================== vvv
// Test pass



namespace {

const pass_data pass_data_ctyler =
{
  GIMPLE_PASS, /* type */
  "ctyler", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  PROP_cfg, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_ctyler : public gimple_opt_pass
{
public:
  pass_ctyler (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_ctyler, ctxt)
  {}

  /* opt_pass methods: */
  bool gate (function *)  final override {
  	return 1; // always execute pass
  }
  unsigned int execute (function *) final override;

}; // class pass_ctyler

unsigned int pass_ctyler::execute(function *)
{
    struct cgraph_node *node;
    int func_cnt = 0;
    int *stmt_counts = (int *)xmalloc(30 * sizeof(int));

    FOR_EACH_FUNCTION(node)
    {
	int stmt_cnt = 0;
        if (dump_file)
        {
	    if (node)
            {
            	fprintf(dump_file, "Function Name === %s\n", node->name());
            	function *fn = node->get_fun();
            	if (fn)
            	{
                	basic_block bb;
                	FOR_EACH_BB_FN(bb, fn) 
			{
				for (gimple_stmt_iterator gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
          			{
            				gimple *g = gsi_stmt (gsi);
            				stmt_cnt++;
                			print_gimple_stmt (dump_file, g, 0, TDF_VOPS|TDF_MEMSYMS);
              			}		
			}

		stmt_counts[func_cnt]= stmt_cnt;
		fprintf(dump_file, "Statement Count: %d \n", stmt_cnt);
		func_cnt++;
    		}
	    }
	}
    }

    if (dump_file)
    {
        fprintf(dump_file, "\n\n#### End ctyler diagnostics, start regular dump of current gimple ####\n\n\n");
    	for (int i = 0; i < func_cnt; i++)
        {
            fprintf(dump_file, "Function %d: %d statements\n", i + 1, stmt_counts[i]);
        }
    }

    free(stmt_counts);
    
    return 0;
}



} // namespace

gimple_opt_pass *
make_pass_ctyler (gcc::context *ctxt)
{
  return new pass_ctyler (ctxt);
}

// =================================================== ^^^
