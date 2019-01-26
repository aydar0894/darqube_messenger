SELECT src, sent, dest, received, tin.hash, time FROM
    (SELECT pubkey as src, time, sent, hash  FROM tx_in
     WHERE pubkey=? limit 100 offset ?) tin
LEFT JOIN
    (SELECT hash, pubkey as dest, received FROM tx_out) tout
ON tin.hash = tout.hash
UNION
SELECT src, sent, dest, received, tin.hash, time FROM
    (SELECT pubkey AS dest, hash, time, received FROM tx_out
    WHERE pubkey=? limit 100 offset ?) tout
LEFT JOIN
    (SELECT hash, pubkey AS src, sent FROM tx_in) tin
ON tin.hash = tout.hash

-- SQLS

-- All related pubkeys

-- SELECT distinct pubkey from
-- ((SELECT tout.pubkey FROM
--     (SELECT pubkey, hash  FROM tx_in
--      WHERE pubkey='1FW8mGV7ddaeHBQtGk33NMWs8tAExjAqUA') tin
-- LEFT JOIN
--     (SELECT hash, pubkey FROM tx_out) tout
-- ON tin.hash = tout.hash)
-- UNION
-- (SELECT tin.pubkey FROM
--     (SELECT pubkey, hash, time, received FROM tx_out
--     WHERE pubkey='1FW8mGV7ddaeHBQtGk33NMWs8tAExjAqUA') tout
-- LEFT JOIN
--     (SELECT hash, pubkey, sent FROM tx_in) tin
-- ON tin.hash = tout.hash)) res

-- ///
