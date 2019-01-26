
-- All related pubkeys

SELECT distinct pubkey from
((SELECT tout.pubkey FROM
    (SELECT pubkey, hash  FROM tx_in
     WHERE pubkey='1FW8mGV7ddaeHBQtGk33NMWs8tAExjAqUA') tin
LEFT JOIN
    (SELECT hash, pubkey FROM tx_out) tout
ON tin.hash = tout.hash)
UNION
(SELECT tin.pubkey FROM
    (SELECT pubkey, hash, time, received FROM tx_out
    WHERE pubkey='1FW8mGV7ddaeHBQtGk33NMWs8tAExjAqUA') tout
LEFT JOIN
    (SELECT hash, pubkey, sent FROM tx_in) tin
ON tin.hash = tout.hash)) res

-- ///
