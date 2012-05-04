namespace CCCount{
	
	
	
	public static class Utils
	{
		/// <summary>
		/// convert a surrogate pair to its corresponding unicode scalar value in uint type.
		/// </summary>
		/// <param name="high">
		/// A <see cref="System.Char"/>
		/// </param>
		/// <param name="low">
		/// A <see cref="System.Char"/>
		/// </param>
		/// <returns>
		/// A <see cref="System.UInt32"/>
		/// </returns>
		public static uint SurrogatesToUint(char high,char low)
		{
			const int shift = 10;
			return 0x10000u + ((uint)(high - 0xd800u) << shift) + (low - 0xdc00u);
		}
		
		/// <summary>
		/// covert a unicode point value to a surrogate pair
		/// </summary>
		/// <param name="u">
		/// A <see cref="System.UInt32"/>
		/// </param>
		/// <returns>
		/// A <see cref="Surrogates"/>
		/// </returns>
		public static string UintToSurrogates (uint u)
		{
			u -= 0x10000u;
			const int shift = 10;			
			char high = (char)((u >> shift) + 0xD800u);
			char low = (char)((u & 0x00003ffu) + 0xDC00u);
			return new string (new char[]{high,low});
		}
	}
}
