library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity seven_segment_coder is
			port(
			sev_seg : out std_logic_vector(7 downto 0);
			binary : in std_logic_vector(3 downto 0)
			);
end seven_segment_coder;

architecture rtl of seven_segment_coder is

begin
		
		process (binary) is
		begin
			case binary is
				when "0000" =>
					sev_seg <= "11000000"; -- 0
				when "0001" =>
					sev_seg <= "11111001"; -- 1
				when "0010" =>
					sev_seg <= "10100100"; -- 2
				when "0011" =>
					sev_seg <= "10110000"; -- 3
				when "0100" =>
					sev_seg <= "10011001"; -- 4
				when "0101" =>
					sev_seg <= "10010010"; -- 5
				when "0110" =>
					sev_seg <= "10000010"; -- 6
				when "0111" =>
					sev_seg <= "11111000"; -- 7
				when "1000" =>
					sev_seg <= "10000000"; -- 8
				when "1001" =>
					sev_seg <= "10011000"; -- 9
				when "1010" =>
					sev_seg <= "10001000"; -- A
				when "1011" =>
					sev_seg <= "10000011"; -- b
				when "1100" =>
					sev_seg <= "11000110"; -- C
				when "1101" =>
					sev_seg <= "10100001"; -- d
				when "1110" =>
					sev_seg <= "10000110"; -- E
				when "1111" =>
					sev_seg <= "10001110"; -- F
				when others =>
					null;
			end case;

		end process;
		
		
end rtl;