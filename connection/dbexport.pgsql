--
-- PostgreSQL database dump
--

-- Dumped from database version 13.3
-- Dumped by pg_dump version 13.3

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: russian_size; Type: TABLE; Schema: public; Owner: launcher
--

CREATE TABLE public.russian_size (
    id integer NOT NULL,
    eth_address character varying(42) NOT NULL,
    burning_address_eth character varying(42) NOT NULL,
    burning_address_btc character varying(74) NOT NULL,
    reff_eth_address character varying(42),
    showed_up_on timestamp without time zone NOT NULL,
    f_balance_btc bigint,
    f_balance_eth bigint
);


ALTER TABLE public.russian_size OWNER TO launcher;

--
-- Name: russian_size_id_seq; Type: SEQUENCE; Schema: public; Owner: launcher
--

CREATE SEQUENCE public.russian_size_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.russian_size_id_seq OWNER TO launcher;

--
-- Name: russian_size_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: launcher
--

ALTER SEQUENCE public.russian_size_id_seq OWNED BY public.russian_size.id;


--
-- Name: russian_size id; Type: DEFAULT; Schema: public; Owner: launcher
--

ALTER TABLE ONLY public.russian_size ALTER COLUMN id SET DEFAULT nextval('public.russian_size_id_seq'::regclass);


--
-- Data for Name: russian_size; Type: TABLE DATA; Schema: public; Owner: launcher
--

COPY public.russian_size (id, eth_address, burning_address_eth, burning_address_btc, reff_eth_address, showed_up_on, f_balance_btc, f_balance_eth) FROM stdin;
1	0xF9cBf7b08f09ED3d4516E8b7A3FCbe4Dc7B3Cd40	0x01b4091244791Ca6b6e82ACC9894d4Af3B93F0eE	tb1qyuq8v6n9cegxy020frqcg7dl4plte0g9awa7l9	\N	2014-03-03 02:03:04	5137004	0
2	0x570215116714E113592ac8ef87C6ABfd176d705e	0xe1CcF78C6E26c45c4e4A5584C19a2e5b2009536E	tb1qcqqpm3y7fzjcmlgu3faun5d543k5yy6jm0lpx3	\N	2014-03-03 02:03:04	7000	0
\.


--
-- Name: russian_size_id_seq; Type: SEQUENCE SET; Schema: public; Owner: launcher
--

SELECT pg_catalog.setval('public.russian_size_id_seq', 2, true);


--
-- Name: russian_size russian_size_burning_address_btc_key; Type: CONSTRAINT; Schema: public; Owner: launcher
--

ALTER TABLE ONLY public.russian_size
    ADD CONSTRAINT russian_size_burning_address_btc_key UNIQUE (burning_address_btc);


--
-- Name: russian_size russian_size_burning_address_eth_key; Type: CONSTRAINT; Schema: public; Owner: launcher
--

ALTER TABLE ONLY public.russian_size
    ADD CONSTRAINT russian_size_burning_address_eth_key UNIQUE (burning_address_eth);


--
-- Name: russian_size russian_size_eth_address_key; Type: CONSTRAINT; Schema: public; Owner: launcher
--

ALTER TABLE ONLY public.russian_size
    ADD CONSTRAINT russian_size_eth_address_key UNIQUE (eth_address);


--
-- Name: russian_size russian_size_pkey; Type: CONSTRAINT; Schema: public; Owner: launcher
--

ALTER TABLE ONLY public.russian_size
    ADD CONSTRAINT russian_size_pkey PRIMARY KEY (id);


--
-- PostgreSQL database dump complete
--

