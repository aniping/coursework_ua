
//
// This file contains the interface for the joins.
// We name the two relations being joined as R and S.
//

class nested_loops_join : public Iterator
{
 public:
   nested_loops_join(AttrType    in1[],      // Array containing field types of R.
                     int     len_in1,        // # of columns in R.
                     short   t1_str_sizes[],
                     AttrType    in2[],      // Array containing field types of S.
                     int     len_in2,        // # of columns in S.
                     short   t2_str_sizes[],

                     int     amt_of_mem,     // IN PAGES

                     Iterator *    am1,      // access method for left i/p to join
                     const char* relationName,// access method for right i/p to join

                     CondExpr **outFilter,   // Ptr to the output filter
                     CondExpr **rightFilter, // Ptr to filter applied on right i/p
                     FldSpec  * proj_list,
                     int        n_out_flds,
                     Status   & s
                    );
   Status get_next(Tuple * &tuple);          // The tuple is returned.

  ~nested_loops_join();
};


// o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o

class index_nested_loop : public Iterator
{
 public:
   index_nested_loop(AttrType    in1[],      // Array containing field types of R.
                     int     len_in1,        // # of columns in R.
                     short   t1_str_sizes[],
                     AttrType    in2[],      // Array containing field types of S.
                     int     len_in2,        // # of columns in S.
                     short   t2_str_sizes[],

                     int     join_col_in1,   // The col of R to be joined with
                     int     join_col_in2,   // the col of S.

                     int     amt_of_mem,     // IN PAGES

                     Iterator *    am1,      // access method for left input to join.
                     IndexType in_type,      // the index type
                     const char*  index_name,// access method for right input to join.
                     const char*  relationName,// access method for right input to join.

                     CondExpr * * outFilter, // Ptr to the output filter
                     CondExpr * * rightFilter,// Ptr to a filter applied on the right i/p
                     FldSpec  * proj_list,
                     int        nOutFlds,
                     int indexOnly,
                     Status   & s
                    );

  ~index_nested_loop();

   Status get_next(Tuple * &tuple);          // The tuple is returned.
};

// ============================================================================
//
class sort_merge : public Iterator
{
 public:
   sort_merge(AttrType    in1[],         // Array containing field types of R.
              int     len_in1,           // # of columns in R.
              short   t1_str_sizes[],
              AttrType    in2[],         // Array containing field types of S.
              int     len_in2,           // # of columns in S.
              short   t2_str_sizes[],
              
              int     join_col_in1,      // The col of R to be joined with
              int      sortFld1Len,
              int     join_col_in2,      // the col of S.
              int      sortFld2Len,
              
              int     amt_of_mem,        // IN PAGES

              Iterator *    am1,         // access method for left input to join.
              Iterator *    am2,         // access method for right input to join.

              int     in1_sorted,        // is am1 sorted?
              int     in2_sorted,        // is am2 sorted?
              TupleOrder order,

              CondExpr * * outFilter,    // Ptr to the output filter
              FldSpec  * proj_list,
              int        nOutFlds,
              Status   & s
             );

  ~sort_merge();

   Status get_next(Tuple * &tuple);      // The tuple is returned.
};


