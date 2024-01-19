library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity exc4 is
			port(
			reset : in std_logic;
			update : in std_logic;
			clk : in std_logic;
			switches_in : in std_logic_vector(9 downto 0);
			leds_out : out std_logic_vector(9 downto 0)
			);
end exc4;

architecture rtl of exc4 is
	signal value_to_be_added : unsigned(9 downto 0) := (others => '0'); --Alustetaan arvot 0 ainakin simulaatiossa ledit eivät näin ole tuntemattomassa tilassa 
	signal output : unsigned(31 downto 0) := (others => '0');
begin	

		process (reset,clk) is
			begin
			if (reset = '0') then			--jos reset on 1 asetetaan ledit takasin 0 ja samoin ynnättävä numero
				output <= "00000000000000000000000000000000";
				value_to_be_added <= "0000000000";
			elsif (update = '0') then			--jos update on 1 asetetaan uusi arvo ynnättäväksi
				value_to_be_added <= unsigned(switches_in);
			elsif rising_edge(clk) then		--jos on nouseva reuna päivitetään ledit uudella arvolla
				output <= output + value_to_be_added;
			end if;
		end process;
		leds_out <= std_logic_vector(output(9 downto 0)); --jotta nähdään simulaatiossa jotain vaihtelua laitetaan 10 LSB
																			-- 10 korkeinta siis olisi output(31 downto 22)
end rtl;