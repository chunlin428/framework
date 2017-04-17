namespace samchon.library
{
	/**
	 * Case generator.
	 * 
	 * {@link ICaseGenerator} is an interface for case generators being used like a matrix.
	 * <ul>
	 * 	<li> A x B x ... x Z -> {@link CartesianProduct} </li>
	 *  <li> n��r(n^r) -> {@link RepeatedPermutation} </li>
	 *  <li> nPr -> {@link Permutation} </li>
	 *  <li> n! -> {@link Factorial} </li>
	 * </ul>
	 * 
	 * @author Jeongho Nam <http://samchon.org>
	 */
	export interface ICaseGenerator extends Iterable<Array<number>>
	{
		/**
		 * Get size of all cases.
		 *
		 * @return Get a number of the all cases.
		 */
		size(): number;

		/**
		 * Get index'th case.
		 *
		 * @param index Index number
		 * @return The row of the index'th in combined permuation case
		 */
		at(index: number): Array<number>;
	}
}