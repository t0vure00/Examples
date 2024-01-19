library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity exc5 is
			port(
			clk : in std_logic;
			reset : in std_logic;
			up_down_sw : in std_logic;
			sw : in std_logic_vector(7 downto 0);
			leds : out std_logic_vector(7 downto 0);
			seg0 : out std_logic_vector(7 downto 0);
			seg1 : out std_logic_vector(7 downto 0)
			);
end exc5;

architecture rtl of exc5 is
		signal output : unsigned(31 downto 0) := (others => '0');
		
		signal segment0 : std_logic_vector(7 downto 0) := (others => '0');
		signal segment1 : std_logic_vector(7 downto 0) := (others => '0');
		
		component seven_segment_coder is
			port(
			sev_seg : out std_logic_vector(7 downto 0);
			binary : in std_logic_vector(3 downto 0)
			);
		end component;
begin

		leds <= std_logic_vector(output(7 downto 0)); -- 31 downto 24, alkuperäiset, mutta questa simulaatiota
		seg0 <= segment0;										 -- varten LSB, että näkyy vaihtelua helpommin
		seg1 <= segment1;
		
		bin_to_7seg0 : seven_segment_coder port map(segment0, std_logic_vector(output(3 downto 0))); -- 27 downto 24
		bin_to_7seg1 : seven_segment_coder port map(segment1, std_logic_vector(output(7 downto 4))); -- 31 downto 28
		
		counter : process (clk,reset, up_down_sw)
		begin
		if (reset = '0') then
			if(up_down_sw = '1') then
				output <= (others => '0');
			elsif(up_down_sw = '0') then
				output <= (others => '1');
			end if;
		elsif rising_edge(clk) then
			if(up_down_sw = '1') then
				if(output(7 downto 0) < unsigned(sw)) then -- 31 downto 24
					output <= output + 1;
				end if;
			elsif(up_down_sw = '0') then
				if(output > "00000000000000000000000000000000") then
					output <= output - 1;
				end if;
			end if;
		end if;
		
		end process;
		
		
		
		
end rtl;